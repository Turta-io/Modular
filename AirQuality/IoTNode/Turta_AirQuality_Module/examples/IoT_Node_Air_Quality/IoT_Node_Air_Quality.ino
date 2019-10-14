#include <Turta_AirQuality_Module.h>

// Create Air Quality Sensor instance.
Turta_AirQuality_Module aq;

void setup() {
  // Initialize Air Quality Sensor.
  aq.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);

  // Delay for the sensor stabilization.
  delay(5000);
}

void loop() {
  // Read eCO2.
  int eco2 = aq.readECO2();
  Serial.print("eCO2: ");
  Serial.print(eco2);
  Serial.println("ppm");

  // Read TVOC.
  int tvoc = aq.readTVOC();
  Serial.print("TVOC: ");
  Serial.print(tvoc);
  Serial.println("ppb");

  // Print Air Quality.
  Serial.print("Air Quality: ");
  if (eco2 < 700) Serial.println("Excellent");
  else if (eco2 < 900) Serial.println("Good");
  else if (eco2 < 1100) Serial.println("Fair");
  else if (eco2 < 1600) Serial.println("Mediocre - Contaminated indoor air");
  else Serial.println("Bad - Heavily contaminated indoor air");

  // Print Air Quality Index Level.
  Serial.print("AQI Level: ");
  if (tvoc < 55) Serial.println("Good");
  else if (tvoc < 71) Serial.println("Moderate");
  else if (tvoc < 86) Serial.println("Unhealthy for Sensitive Groups");
  else if (tvoc < 106) Serial.println("Very Unhealthy");
  else if (tvoc < 201) Serial.println("Moderate");
  else Serial.println("Hazardous");

  // Print an empty line.
  Serial.println("");

  // Delay 2000ms = 2 seconds.
  delay(2000);
}