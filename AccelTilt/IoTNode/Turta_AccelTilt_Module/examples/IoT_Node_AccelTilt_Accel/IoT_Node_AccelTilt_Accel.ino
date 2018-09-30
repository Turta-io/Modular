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
  // Read X-Axis.
  double xAxisG = accel.readXAxis();
  Serial.print("X: ");
  Serial.println(xAxisG);

  // Read Y-Axis.
  double yAxisG = accel.readYAxis();
  Serial.print("Y: ");
  Serial.println(yAxisG);

  // Read Z-Axis.
  double zAxisG = accel.readZAxis();
  Serial.print("Z: ");
  Serial.println(zAxisG);

  // Read X, Y, Z-Axis at one shot.
  double x, y, z;
  accel.readXYZAxis(x, y, z);
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.print(", Z: ");
  Serial.println(z);

  // Print an empty line.
  Serial.println("");

  // Delay 100ms.
  delay(100);
}

