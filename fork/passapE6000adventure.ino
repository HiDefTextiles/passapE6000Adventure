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
		interrupted = false;
		switch (state)
		{
		case 40:
		{
			counter--;
			digitalWrite(PIN_NEEDLE_RTL, array[abs(counter)]);
			Serial.println(counter);
		}
		break;
		case 41:
		{
			// digitalWrite(PIN_NEEDLE_LTR, 0);
			counter++;
			digitalWrite(PIN_NEEDLE_LTR, array[abs(counter)]);
			Serial.println(counter);
		}
		default:
			break;
		}
	}
}

// Interrupt
void interrupt_CSENSE()
{
	int stateNew = ((crefNow + 1) * 10) + csenseNow;
	if (state == stateNew)
	{
		return;
	}
	interrupted = true; // flag

	crefNow = digitalRead(PIN_CREF);
	csenseNow = digitalRead(PIN_CSENSE);
	state = ((crefNow + 3) * 10) + csenseNow;
}
