#include <Turta_Environmental_Module.h>

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
  // Read temperature, pressure, and humidity in one shot. 
  double temperature, pressure, humidity;
  en.readTPH(temperature, pressure, humidity);

  // Print temperature.
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Print relative humidity.
  Serial.print("Humidity: ");
  Serial.println(humidity);

  // Print pressure.
  Serial.print("Pressure: ");
  Serial.println(pressure);

  // Print an empty line.
  Serial.println("");

  // Delay 5000ms = 5 seconds.
  delay(5000);
}

