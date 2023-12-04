#include <Encoder.h>
#include <Wire.h>
#include "DS1307.h"
#include "rgb_lcd.h"

// Global vars for StateMachine
bool stateLocked = false;
int currentState = 0;

// Global vars for ButtonEvent
const int buttonPin = 6;
bool lastButtonState = false;
bool buttonClicked = false;
const int buttonCooldownMs = 100;
long lastButtonClicktime = millis();

// Encoder setup
Encoder myEnc(3, 2);
int encoderValue = 0;
int prevEncoderValue = 0;

// LCD definition and variables
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 255;
const int colorB = 255;

// Clock definition
DS1307 clock; //define a object of DS1307 class

void setup()
{
	Serial.begin(9600);

	// Set button pin
	pinMode(buttonPin, INPUT);

	// Clock setup
	clock.begin();

	// LCD setup
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
}

void loop()
{
	// Update buttonClicked
	ButtonEvent();

	// Run StateMachine to run program functionality
	StateMachine();
}

// Helper functions
String SetZero(uint8_t number)
{
	/*
		This function is for formatting date and time. It decides if there should be an extra 0 in front of a number.
	*/
	if (number > 9)
	{
		return "";
	} else {
		return "0";
	}
}

void UpdateEncoderValue()
{
	prevEncoderValue = encoderValue;
	encoderValue = myEnc.read();
}

// Program stucture functions
void StateMachine()
{
	bool stateUpdated = false;
	if (stateLocked == false)
	{
		UpdateEncoderValue();
		currentState = abs((encoderValue / -4) % 4);
		if (abs((prevEncoderValue / -4) % 4) != currentState)
		{
			stateUpdated = true;
		}
	}
	// Clear lcd if state has changed
	if (stateUpdated == true)
	{
		lcd.clear();
	}

	// Choose and run state
	switch (currentState)
	{
		case 0:
			Clock();
			break;

		default:
			// If currentState doesn't exist
			lcd.setCursor(0, 0);
			lcd.print("Error!");
			lcd.setCursor(0, 1);
			lcd.print("Invalid state!");
			break;
  	}
}

// Functionality functions
void Clock()
{
	/*
		This function will display time and date from RTC on lcd.
	*/
	// Get current time from RTC
	clock.getTime();
	// Print time on lcd
	lcd.setCursor(0, 0);
	lcd.print(SetZero(clock.hour));
	lcd.print(clock.hour);
	lcd.print(":");
	lcd.print(SetZero(clock.minute));
	lcd.print(clock.minute);
	lcd.print(":");
	lcd.print(SetZero(clock.second));
	lcd.print(clock.second);
	// Print date on lcd
	lcd.setCursor(0, 1);
	lcd.print(SetZero(clock.dayOfMonth));
	lcd.print(clock.dayOfMonth);
	lcd.print("/");
	lcd.print(SetZero(clock.month));
	lcd.print(clock.month);
	lcd.print("/");
	lcd.print(clock.year+2000);
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