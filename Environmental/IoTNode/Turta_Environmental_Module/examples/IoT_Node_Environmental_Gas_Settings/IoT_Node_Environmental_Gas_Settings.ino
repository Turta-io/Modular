#include <Turta_Environmental_Module.h>

// Create Environmental Sensor instance.
Turta_Environmental_Module en;

void setup() {
  // Initialize Environmental Sensor.
  // Heat Duration: 150secs, Heat Temperature: 300C
  en.beginAdvanced(150, 300);

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read gas resistance.
  double gasResistance = en.readGasResistance();
  Serial.print("Gas Resistance: ");
  Serial.println(gasResistance);

  // Delay 5000ms = 5 seconds.
  delay(5000);
}

