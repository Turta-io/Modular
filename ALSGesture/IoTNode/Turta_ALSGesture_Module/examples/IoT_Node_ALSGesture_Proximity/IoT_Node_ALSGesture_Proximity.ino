#include <Turta_ALSGesture_Module.h>

// Create ALS Gesture Sensor instance.
Turta_ALSGesture_Module als;

void setup() {
  // Initialize ALS Gesture Sensor.
  als.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read proximity.
  short resultP = als.readProximity();
  Serial.print("Proximity: ");
  Serial.println(resultP);

  // Print an empty line.
  Serial.println("");

  // Delay 1000ms = 1 seconds.
  delay(1000);
}

