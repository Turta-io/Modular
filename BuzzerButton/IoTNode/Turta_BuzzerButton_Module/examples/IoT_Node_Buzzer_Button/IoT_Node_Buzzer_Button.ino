#include <Turta_BuzzerButton_Module.h>
#define LED 13

// Global variables.
bool buttonPressed = false;
bool oldButtonState = false;

// Create Buzzer Button instance.
Turta_BuzzerButton_Module bb;

void setup() {
  // Initialize onboard LED.
  pinMode(LED, OUTPUT);

  // Initialize Buzzer Button.
  bb.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read button state.
  buttonPressed = bb.buttonRead();

  // On button pressed / released.
  if (oldButtonState != buttonPressed) {
    // Set LED state.
    digitalWrite(LED, buttonPressed ? HIGH : LOW);

    // Write button state to the serial monitor.
    Serial.println(buttonPressed ? "Button pressed." : "Button released.");

    // Play tone if button is pressed.
    if (buttonPressed) {
      // Buzzer tone alternative 1:
      // Play 5000Hz tone in 192/255 duty cycle for 100ms.
      bb.buzzerTonePeriod(5000, 192, 100);
    }

    // Play tone if button is released.
    else {
      // Buzzer tone alternative 2:
      // Play 2000Hz tone in 64/255 duty cycle until stopped.
      bb.buzzerTone(2000, 64);
      
      // Delay 100ms.
      delay(100);
      
      // Stop the buzzer.
      bb.buzzerStop();
    }
  }

  // Store last button state.
  oldButtonState = buttonPressed;

  // Delay 10ms.
  delay(10);
}
