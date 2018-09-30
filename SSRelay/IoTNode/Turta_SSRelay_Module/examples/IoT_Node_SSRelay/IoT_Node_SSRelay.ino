#include <Turta_SSRelay_Module.h>

// Create SS Relay instance.
Turta_SSRelay_Module ssr;

void setup() {
  // Initialize SS Relay.
  ssr.begin();

  // Configure serial port.
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // Read relay state.
  bool relayState = ssr.relayRead();

  // Toggle relay.
  ssr.relayWrite(!relayState);

  // Write relay state to serial monitor.
  Serial.print("Relay state: ");
  Serial.println(ssr.relayRead() ? "On." : "Off.");
  
  // Delay 10000ms = 10 seconds.
  delay(10000);
}
