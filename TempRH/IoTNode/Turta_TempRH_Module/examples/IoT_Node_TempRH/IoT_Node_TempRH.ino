#include <Turta_TempRH_Module.h>

// Create Temperature & RH Sensor instance.
Turta_TempRH_Module th;

void setup() {
  // Initialize Temperature & RH Sensor.
  th.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read temperature.
  double temperature = th.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Read relative humidity.
  double humidity = th.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humidity);

  // Print an empty line.
  Serial.println("");

  // Delay 1000ms = 1 seconds.
  delay(1000);
}

