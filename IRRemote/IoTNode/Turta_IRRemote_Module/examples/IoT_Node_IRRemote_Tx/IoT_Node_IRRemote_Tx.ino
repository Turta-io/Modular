#include <Turta_IRRemote_Module.h>
#define LED 13

// Create IR Remote Module instance.
Turta_IRRemote_Module ir;

void setup() {
  // Initialize onboard LED.
  pinMode(LED, OUTPUT);

  // Initialize IR Remote Module.
  ir.begin();
  ir.setReception(false);
  
  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  digitalWrite(LED, HIGH);
  ir.sendNECCommand(1, 2, 3, 252);
  digitalWrite(LED, LOW);

  // Delay 10000ms = 10 seconds.
  delay(10000);
}
