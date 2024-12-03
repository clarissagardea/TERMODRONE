#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Initialize I2C on default pins (GPIO 21 for SDA, GPIO 22 for SCL on ESP32)

  Serial.println("Scanning for I2C devices...");
  byte count = 0;

  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address: 0x");
      Serial.println(i, HEX);
      count++;
    }
  }

  if (count == 0) {
    Serial.println("No I2C devices found.");
  } else {
    Serial.println("I2C scan complete.");
  }
}

void loop() {
  // Nothing here
}
