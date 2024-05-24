// - Constant declarations

#define PIN_CSENSE 2
#define PIN_CREF 4
// Horfir á lásin,
#define PIN_NEEDLE_RTL 5 // Nálirnar koma inn hægra megin
#define PIN_NEEDLE_LTR 6 // Nálarnar koma inn vinstra megin
// Hægri og vinstri séð frá sónarhorni þess sem horfir beint á lásin

// - Variable declarations

// Skynjarar
volatile int csenseNow = 0;
volatile int crefNow = 0;

// sensor change
volatile boolean interrupted = false;
// direction
volatile int state = 0;
// position
volatile int counter = 0;
unsigned long lastCursorChange = 0;
volatile int step = 0;
volatile int sub_step = 0;
// - Setup
// array
// int array[255] = {0, 1, 0, 1, 0,1, 0, 1, 0, 1,0, 1, 0, 1, 0,1, 0, 1, 0, 1,0, 1, 0, 1, 0,1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
int array[13] = {0};
int arraysize = sizeof(array) / sizeof(array[0]);
void setup()
{
	Serial.begin(115200);

	// Skynjarar
	pinMode(PIN_CSENSE, INPUT_PULLUP);
	pinMode(PIN_CREF, INPUT_PULLUP);
	// Interrupt
	attachInterrupt(digitalPinToInterrupt(PIN_CSENSE), interrupt_CSENSE, CHANGE);

	// Seglar
	pinMode(PIN_NEEDLE_LTR, OUTPUT);
	digitalWrite(PIN_NEEDLE_LTR, HIGH);
	pinMode(PIN_NEEDLE_RTL, OUTPUT);
	digitalWrite(PIN_NEEDLE_RTL, HIGH);
}

// -  Loop

void loop()
{
	if (interrupted)
	{
		// Serial.println(state);
		// Serial.println(counter);
		interrupted = false;
		int input = -1;
		int start = (input < 0) ? input + 90 : input + 89;
		int index = counter - 26 - start;
		int value = 1;
		switch (state)
		{
		case 31:
		{
			// counter--;
			if (index + 10 >= 0 && index + 10 < arraysize)
			{
				value = array[index + 10];
			}
			digitalWrite(PIN_NEEDLE_RTL, value);
		}
		break;
		case 40:
		{
			counter--;
		}
		break;
		case 41:
		{
			// digitalWrite(PIN_NEEDLE_LTR, 0);
			counter++;
			if (index >= 0 && index < arraysize)
			{
				value = array[index];
			}
			digitalWrite(PIN_NEEDLE_LTR, value);
		}
		default:
			break;
		}
		Serial.println(index);
	}
}

// Interrupt
void interrupt_CSENSE()
{

	int csenseNew = digitalRead(PIN_CSENSE);
	if (csenseNow != csenseNew)
	{
		crefNow = digitalRead(PIN_CREF);

		interrupted = true; // flag
		csenseNow = csenseNew;
		state = ((crefNow + 3) * 10) + csenseNow;
	}
	// Serial.println(9999999); // til að sjá endurtekningu
}

// 30 0
// 31 0

// 31 0
// 40 -1
// 41 0
// 40 -1
// 41 0
// 40 -1

// byrjar 31 0
// svo mynztur 40 - 41
// byrjar loks 41 - 30
// í 28 erum við komin á fyrstu nálina
// 207 seinasta nál || 206
// 243 auto color endir
// 221 útaf hægri
