#include <Turta_Photocoupler_Module.h>
#define LED 13

// Global variables.
bool input1State = false;
bool input2State = false;
bool oldInput1State = false;
bool oldInput2State = false;

// Create Photocoupler Module instance.
Turta_Photocoupler_Module pc;

void setup() {
  // Initialize onboard LED.
  pinMode(LED, OUTPUT);

  // Initialize Photocoupler Module.
  pc.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read input states.
  input1State = pc.readInput(1);
  input2State = pc.readInput(2);

  if (input1State || input2State) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }

  // On input 1 change.
  if (oldInput1State != input1State) {
    Serial.print("Input 1 State: ");
    Serial.println(input1State ? "High." : "Low.");
  }

  // On input 2 change.
  if (oldInput2State != input2State) {
    Serial.print("Input 2 State: ");
    Serial.println(input2State ? "High." : "Low.");
  }

  // Store last input states.
  oldInput1State = input1State;
  oldInput2State = input2State;

  // Delay 10ms.
  delay(10);
}
