#include <Turta_IRRemote_Module.h>
#define LED 13

// Create IR Remote Module instance.
Turta_IRRemote_Module ir;

void setup() {
  // Initialize onboard LED.
  pinMode(LED, OUTPUT);

  // Initialize IR Remote Module.
  ir.begin();
  ir.setReception(true);
  
  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  if (ir.isMessageReceived()) {
    digitalWrite(LED, HIGH);
    byte dt[4] = {0};
    ir.getData(dt[0], dt[1], dt[2], dt[3]);
    Serial.print(dt[0]);
    Serial.print(" ");
    Serial.print(dt[1]);
    Serial.print(" ");
    Serial.print(dt[2]);
    Serial.print(" ");
    Serial.println(dt[3]);
    digitalWrite(LED, LOW);
  }

  // Delay 10ms.
  delay(10);
}
