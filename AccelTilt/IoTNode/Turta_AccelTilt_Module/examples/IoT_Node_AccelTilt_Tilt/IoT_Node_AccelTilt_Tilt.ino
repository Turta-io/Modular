#include <Turta_AccelTilt_Module.h>

// Create Accel & Tilt Sensor instance.
Turta_AccelTilt_Module accel;

void setup() {
  // Initialize Accel & Tilt Sensor.
  accel.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read Tilt State.
  bool xTilt, yTilt, zTilt;
  accel.readTiltState(xTilt, yTilt, zTilt);
  Serial.print("X: ");
  Serial.print(xTilt ? "Tilt" : "Flat");
  Serial.print(", Y: ");
  Serial.print(yTilt ? "Tilt" : "Flat");
  Serial.print(", Z: ");
  Serial.println(zTilt ? "Tilt" : "Flat");

  // Delay 100ms.
  delay(100);
}

