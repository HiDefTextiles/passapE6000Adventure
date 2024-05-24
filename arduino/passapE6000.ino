
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

char currentPinChangeValue = 0;
char currentPinChangeOppositeValue = 0;

volatile bool interrupted = false;
volatile int state = 0;
volatile int nr = 0;

volatile int csenseNow = 0;
volatile int crefNow = 0;

void setup()
{
	Serial.begin(9600);

	// photo sensors ##virkar
	pinMode(PIN_CSENSE, INPUT_PULLUP);
	pinMode(PIN_CREF, INPUT_PULLUP);

	csenseNow = digitalRead(PIN_CSENSE);
	crefNow = digitalRead(PIN_CREF);
	// photo sensor setup
	attachInterrupt(digitalPinToInterrupt(PIN_CSENSE), interrupt_CSENSE, CHANGE);
	// attachInterrupt(digitalPinToInterrupt(PIN_CREF), interrupt_CREF, CHANGE);
	// Corrected typo
	// Nálar ##virkar
	pinMode(PIN_NEEDLE_RTL, OUTPUT);
	digitalWrite(PIN_NEEDLE_RTL, HIGH);
	pinMode(PIN_NEEDLE_LTR, OUTPUT);
	digitalWrite(PIN_NEEDLE_LTR, HIGH);
}

void loop()
{
	// delay(5000);
	if (interrupted)
	{
		interrupted = false;
		switch (state)
		{
		case 40:
			Serial.println(40);
			break;
		case 41:
			break;
		case 30:
			break;
		case 31:
			break;
		case 20:
			break;
		case 21:
			break;
		case 10:
			break;
		case 11:
			break;
		default:
			break;
		}
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

void interrupt_CSENSE()
{
	interrupted = true;
	crefNow = digitalRead(PIN_CREF);
	csenseNow = digitalRead(PIN_CSENSE);
	state = ((crefNow + 3) * 10) + csenseNow;
}

void interrupt_CREF()
{
	interrupted = true;

	csenseNow = digitalRead(PIN_CSENSE);
	crefNow = digitalRead(PIN_CREF);
	// state = ((crefNow + 1) * 10) + csenseNow;
	state = 20;
}
