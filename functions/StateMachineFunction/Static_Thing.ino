const int buttonPin = 2; // Change this to the pin where your button is connected
int currentState = 1;    // Set the initial state
bool buttonPressed = false;

void setup() {
  // Your setup code here
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); // Assuming the button is connected between the pin and ground
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    buttonPressed = true;
  } else {
    buttonPressed = false;
  }

  // Check the current state and execute corresponding code
  switch (currentState) {
    case 1:
      state1Code();
      break;

    case 2:
      if (buttonPressed) {
        state2Code();
      }
      break;

    // Add more cases as needed

    default:
      // Default case if currentState doesn't match any defined cases
      break;
  }
}

void state1Code() {
  // Code for state 1
  Serial.println("Executing code for State 1");
  // Add your state 1 specific code here
}

void state2Code() {
  // Code for state 2
  Serial.println("Executing code for State 2");
  // Add your state 2 specific code here
}

// Add more stateXCode() functions as needed
