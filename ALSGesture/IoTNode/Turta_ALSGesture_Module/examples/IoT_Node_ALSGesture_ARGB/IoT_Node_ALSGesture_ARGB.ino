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
  // Read ambient light.
  int resultA = als.readAmbientLight();
  Serial.print("Ambient light: ");
  Serial.println(resultA);

  // Read ARGB light.
  // A: Ambient, R: Red, G: Green, B: Blue light.
  int a, r, g, b;
  als.readARGBLight(a, r, g, b);
  Serial.print("A: ");
  Serial.print(a);
  Serial.print(", R: ");
  Serial.print(r);
  Serial.print(", G: ");
  Serial.print(g);
  Serial.print(", B: ");
  Serial.println(b);
  
  // Print an empty line.
  Serial.println("");

  // Delay 1000ms = 1 seconds.
  delay(1000);
}

