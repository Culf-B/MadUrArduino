#include <Encoder.h>
#include <Wire.h>

// Global vars for ButtonEvent
const int buttonPin = 6;
bool lastButtonState = false;
bool buttonClicked = false;
const int buttonCooldownMs = 100;
long lastButtonClicktime = millis();

// Encoder setup
Encoder myEnc(3, 2);
int encoderValue = 0;
int prevEncoderValueCount = 0;
int encoderValueCount = 0;

void setup()
{
	Serial.begin(9600);

	pinMode(buttonPin, INPUT);
}

void loop()
{
	encoderValue = myEnc.read();
	prevEncoderValueCount = encoderValueCount;
	encoderValueCount = encoderValue / -4;

	// Update buttonClicked
	ButtonEvent();
}

void ButtonEvent() 
{
	/*
	This function updates the global variable buttonClicked.
	*/
	// Checks encoder value to work around encoder substep button click.
	if (myEnc.read() % 4 == 0)
	{
		// Read button state from button pin.
		// The value is inverted because the button pin is HIGH when the button is not pressed, and LOW when the button is pressed.
		bool buttonState = !digitalRead(buttonPin);

		if (buttonState == false && lastButtonState == true && millis() > lastButtonClicktime + buttonCooldownMs) // The timing is to add a cooldown to avoid ghost clicking
		{
			// There is a button event
			buttonClicked = true;

			lastButtonClicktime = millis();
		} else
		{
			// There is not a button event
			buttonClicked = false;
		}

		lastButtonState = buttonState;
	} else
	{
		// Encoder substep button click does not count as a button event
		lastButtonState = false;
		buttonClicked = false;
	}
}