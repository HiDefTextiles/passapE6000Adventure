
// ARDUINO test fyrir front lock

// DEFINE INTERRUPTS FOR CSENSE
#define INT_ENCODER 0

// FREMRI LÁS
#define PIN_CSENSE 2
#define PIN_CREF 4
#define PIN_NEEDLE_RTL 5
#define PIN_NEEDLE_LTR 6

#define DIRECTION_UNKOWN 0
#define DIRECTION_LEFT_RIGHT -1
#define DIRECTION_RIGHT_LEFT 1

char currentDirection = DIRECTION_UNKOWN;
char lastDirection = DIRECTION_UNKOWN;

signed int currentCursorPosition = 0;
unsigned long lastCursorChange = 0;
unsigned int currentPatternIndex = 0;
signed int firstNeedle = 0;
signed int offsetCarriage_RTL = 53;
signed int offsetCarriage_LTR = 29;

// int knitPattern[255] = {0};

int knitPattern[255] = {
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

int passapCalibrateArray[8] = {0};
signed int passapCalibratePointer = 0;
static unsigned char passaptestpattern[8] = {1, 0, 1, 1, 0, 0, 0, 1};

char currentPinChangeValue = 0;
char currentPinChangeOppositeValue = 0;
signed int currentCursorPositionLast = 0;

//////////////////////////////////////////////////////////////////////////////
// Knitty Serial Protocol

// Receive commands
#define COM_CMD_PATTERN 'P'
#define COM_CMD_PATTERN_END 'E'
#define COM_CMD_CURSOR 'C'
#define COM_CMD_RESPONSE 'R'
#define COM_CMD_DIRECTION 'D'
#define COM_CMD_DEBUG 'V'
#define COM_CMD_NEW_PATTERN 'N'
#define COM_CMD_FIRST_NEEDLE 'F' // first needle of pattern from right
#define COM_CMD_SEPERATOR ':'
#define COM_CMD_STATUS 'T'

#define COM_CMD_PLOAD_END '\n'

// Parser states
#define COM_PARSE_CMD 0x01
#define COM_PARSE_SEP 0x02
#define COM_PARSE_PLOAD 0x03

unsigned char parserState = COM_PARSE_CMD;

unsigned char parserReceivedCommand = 0;
String parserReceivedPayload = "";
unsigned char patternLength = 0;
unsigned char patternLengthBack = 0;

void setup()
{
	Serial.begin(115200);
	sendCommand(COM_CMD_RESPONSE, "up and running");

	// Setup PHOTO SENSOR pin change interrupt
	pinMode(PIN_CSENSE, INPUT_PULLUP);

	pinMode(PIN_CREF, INPUT_PULLUP);

	attachInterrupt(INT_ENCODER, interruptPinChangeEncoder, CHANGE);

	// Setup Needles
	pinMode(PIN_NEEDLE_RTL, OUTPUT);
	digitalWrite(PIN_NEEDLE_RTL, HIGH);
	pinMode(PIN_NEEDLE_LTR, OUTPUT);
	digitalWrite(PIN_NEEDLE_LTR, HIGH);
}

void loop()
{
	parserSerialStream();
	patternFront();
}

void executeCommand(unsigned char cmd, String payload)
{

	switch (cmd)
	{
	case COM_CMD_PATTERN:
		// Serial.print("P ");
		// Serial.println(patternLength);
		//  sendCommand(COM_CMD_RESPONSE, "PatternLength: " + String(patternLength));
		break;

	case COM_CMD_CURSOR:
		currentCursorPosition = payload.toInt();
		break;

	case COM_CMD_FIRST_NEEDLE:
		firstNeedle = payload.toInt() * 2 - 2;

		sendCommand(COM_CMD_RESPONSE, "FirstNeedle: " + String(firstNeedle));
		break;

	case COM_CMD_STATUS:
		sendCommand(COM_CMD_RESPONSE, "-------------------------");
		sendCommand(COM_CMD_RESPONSE, "Cursorposition: " + String(currentCursorPosition));
		sendCommand(COM_CMD_RESPONSE, "PatternIndex: " + String(currentPatternIndex));
		if (currentDirection == DIRECTION_RIGHT_LEFT)
		{
			sendCommand(COM_CMD_DIRECTION, "RTL");
		}
		else
		{
			sendCommand(COM_CMD_DIRECTION, "LTR");
		}
		sendCommand(COM_CMD_RESPONSE, "-------------------------");

		break;
	}
}

void sendCommand(unsigned char cmd, String payload)
{
	Serial.write(cmd);
	Serial.write(COM_CMD_SEPERATOR);
	Serial.print(payload);
	Serial.write("\n");
}

void parserSerialStream()
{

	if (Serial.available() == 0)
	{
		return;
	}

	char buffer = Serial.read();

	switch (parserState)
	{

	case COM_PARSE_CMD:
		parserState = COM_PARSE_SEP;
		parserReceivedCommand = buffer;
		parserReceivedPayload = "";
		if (buffer == COM_CMD_PATTERN)
		{
			patternLength = 0;
		}
		break;

	case COM_PARSE_SEP:

		// We're awaiting a seperator here, if not, back to cmd
		if (buffer == COM_CMD_SEPERATOR)
		{
			parserState = COM_PARSE_PLOAD;
			break;
		}

		parserState = COM_PARSE_CMD;
		break;

	case COM_PARSE_PLOAD:

		if (buffer == COM_CMD_PLOAD_END)
		{

			executeCommand(parserReceivedCommand, parserReceivedPayload);
			parserState = COM_PARSE_CMD;

			////sendCommand(COM_CMD_RESPONSE, "Received");
			break;
		}

		if (parserReceivedCommand == COM_CMD_PATTERN)
		{
			// Change state because the E6000 set the needle at '0'
			knitPattern[patternLength] = (buffer == '0') ? 1 : 0;
			// reverse pattern
			// knitPattern_back[patternLength] = (buffer == '1')? 1 : 0;

			// Serial.println(String(knitPattern_back[patternLength])+String(patternLength));
			patternLength += 1;
		}
		else
		{
			parserReceivedPayload += buffer;
		}

		break;
	}
}

void setNeedle_RTL(char state)
{
	// Attention E6000 sets needle by 0
	digitalWrite(PIN_NEEDLE_RTL, state);
}

void setNeedle_LTR(char state)
{
	// Attention E6000 sets needle by 0
	digitalWrite(PIN_NEEDLE_LTR, state);
}

void patternFront()
{

	// react only if there is new cursorposition
	if (currentCursorPositionLast != currentCursorPosition)
	{

		// RTL
		if (currentDirection == DIRECTION_RIGHT_LEFT)
		{
			int patternPositionRTL = currentCursorPosition - (firstNeedle + offsetCarriage_RTL);
			// set needles in absolute position
			if (patternPositionRTL > 0 && patternPositionRTL <= patternLength * 2)
			{
				setNeedle_RTL(knitPattern[((patternLength * 2 - patternPositionRTL) / 2)]);
			}
			else
			{
				setNeedle_RTL(1);
			}
			// send pattern End
			if (patternPositionRTL == patternLength * 2 + 1)
			{
				sendCommand(COM_CMD_PATTERN_END, "1");
			}
		}

		// LTR
		if (currentDirection == DIRECTION_LEFT_RIGHT)
		{

			int patternPositionLTR = currentCursorPosition - (firstNeedle + offsetCarriage_LTR);
			if (patternPositionLTR > 0 && patternPositionLTR <= patternLength * 2)
			{
				setNeedle_LTR(knitPattern[((patternLength * 2 - patternPositionLTR) / 2)]);
			}
			else
			{
				setNeedle_LTR(1);
			}
			// send pattern End
			if (patternPositionLTR == 0)
			{
				sendCommand(COM_CMD_PATTERN_END, "1");
			}
		}

		// store last CursorPosition
		currentCursorPositionLast = currentCursorPosition;
	}
}

void interruptPinChangeEncoder()
{
	// Serial.println(999);
	unsigned long now = micros();
	Serial.println(now);
	if (now - lastCursorChange < 1000)
	{
		lastCursorChange = now;
		return;
	}
	lastCursorChange = now;

	currentPinChangeValue = digitalRead(PIN_CSENSE);
	currentPinChangeOppositeValue = digitalRead(PIN_CREF);

	//    Serial.print(String(0+currentPinChangeValue));
	//    Serial.print("-");
	//    Serial.println(String(0+currentPinChangeOppositeValue));

	// Determine direction
	if (currentPinChangeValue == currentPinChangeOppositeValue)
	{ // ef sama þá vinstri ti hægri
		currentDirection = DIRECTION_LEFT_RIGHT;
	}
	else
	{
		currentDirection = DIRECTION_RIGHT_LEFT;
	}

	// RTL = 1, LTR = -1
	currentCursorPosition += currentDirection;

	// AutoCalibrate
	passapCalibrateArray[passapCalibratePointer & 0x07] = currentPinChangeValue;
	passapCalibrateArray[(passapCalibratePointer + 1) & 0x07] = currentPinChangeOppositeValue;
	/*
	// uncomment for finding the calibration sequence -> store in 'passaptestpattern'
	  for (int i = 0; i < 8; i++) {

	  if ((passapCalibratePointer - i) >= -1) {

		Serial.print(passapCalibrateArray[(passapCalibratePointer - i+1)]);

	  }
	  else {

	   Serial.print(passapCalibrateArray[(passapCalibratePointer - i+9)]);

	  }
	  }
	Serial.println("");
	*/
	//
	int found = 1;
	for (int i = 0; i < 8; i++)
	{

		if ((passapCalibratePointer - i) >= -1)
		{

			// Serial.print((passapCalibratePointer - i+1));
			if (passapCalibrateArray[(passapCalibratePointer - i + 1) & 0x07] !=
				passaptestpattern[i])
			{
				found = 0;
				break;
			}
		}
		else
		{

			//  Serial.print((passapCalibratePointer - i + 9));
			if (passapCalibrateArray[(passapCalibratePointer - i + 9) & 0x07] !=
				passaptestpattern[i])
			{
				found = 0;
				break;
			}
		}
	}

	// Serial.println("end");
	if (found)
	{
		sendCommand(COM_CMD_RESPONSE, "fc");
		// calibrate
		currentCursorPosition = -2;
	}
	/*
	  Serial.print("point:");
	  Serial.println(passapCalibratePointer);
	  Serial.println("");
	*/
	passapCalibratePointer = passapCalibratePointer + 2;
	if (passapCalibratePointer >= 8)
	{
		passapCalibratePointer = 0;
	}
}
