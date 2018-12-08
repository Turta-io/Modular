#include <Turta_Environmental_Module.h>

// Global Variables
double meanSeaLevelPressureInBar = 1020.0;

// Create Environmental Sensor instance.
Turta_Environmental_Module en;

void setup() {
  // Initialize Environmental Sensor.
  en.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read temperature.
  double temperature = en.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Read relative humidity.
  double humidity = en.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humidity);

  // Read pressure.
  double pressure = en.readPressure();
  Serial.print("Pressure: ");
  Serial.println(pressure);

  // Calculate altitude.
  double altitude = en.calcAltitude(meanSeaLevelPressureInBar);
  Serial.print("Altitude: ");
  Serial.println(altitude);

  // Read gas resistance.
  double gasResistance = en.readGasResistance();
  Serial.print("Gas Resistance: ");
  Serial.println(gasResistance);

  // Print an empty line.
  Serial.println("");

  // Delay 5000ms = 5 seconds.
  delay(5000);
}
