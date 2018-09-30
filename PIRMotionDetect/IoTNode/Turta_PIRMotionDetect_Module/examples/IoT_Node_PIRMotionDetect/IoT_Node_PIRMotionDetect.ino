#include <Turta_PIRMotionDetect_Module.h>
#define LED 13

// Global variables.
bool motionDetected = false;
bool oldMotionState = false;

// Create PIR Motion Detect instance.
Turta_PIRMotionDetect_Module pir;

void setup() {
  // Initialize onboard LED.
  pinMode(LED, OUTPUT);

  // Initialize PIR.
  pir.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read PIR state.
  motionDetected = pir.pirRead();

  // On motion detected / no motion.
  if (oldMotionState != motionDetected) {
    digitalWrite(LED, motionDetected ? HIGH : LOW);
    Serial.println(motionDetected ? "Motion detected." : "No motion.");
  }

  // Store last motion state.
  oldMotionState = motionDetected;

  // Delay 10ms.
  delay(10);
}
