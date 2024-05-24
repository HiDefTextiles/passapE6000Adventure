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
int array[255] = {
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

void setup()
{
	Serial.begin(9600);

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
		Serial.println(state);
		// Serial.println(counter);
		interrupted = false;
		switch (state)
		{
		case 31:
		{
			// counter--;
			digitalWrite(PIN_NEEDLE_RTL, array[abs(counter)]);
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
			digitalWrite(PIN_NEEDLE_LTR, array[abs(counter)]);
		}
		default:
			break;
		}
		Serial.println(counter);
	}
}

// Interrupt
void interrupt_CSENSE()
{
	unsigned long now = micros();
	// Serial.println(now);
	if (lastCursorChange - now < 1000)
	{
		lastCursorChange = now;

		return;
	}
	// lastCursorChange = now;
	crefNow = digitalRead(PIN_CREF);
	csenseNow = digitalRead(PIN_CSENSE);
	int stateNew = ((crefNow + 3) * 10) + csenseNow;
	if (state != stateNew)
	{
		interrupted = true; // flag
		state = stateNew;
	}
	Serial.println(9999999); // til að sjá endurtekningu
}

// 30 0
// 31 0
