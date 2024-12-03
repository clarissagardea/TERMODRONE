#include <Wire.h>
#include <Adafruit_MLX90640.h>

// Crear una instancia de la cámara
Adafruit_MLX90640 mlx90640;

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  // Set the built-in LED pin as output
  digitalWrite(LED_BUILTIN, HIGH);  // Turn on the LED to indicate the code is running
  while (!Serial);
  Serial.println("Starting I2C...");
  
  Wire.begin();
  
  if (!mlx90640.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("Failed to initialize MLX90640. Check connections.");
    digitalWrite(LED_BUILTIN, LOW);  // Turn off the LED if initialization fails
    while (1);
  } else {
    Serial.println("MLX90640 initialized successfully.");
    digitalWrite(LED_BUILTIN, LOW);  // Turn off the LED when initialization succeeds
  }
  
  Serial.println("Setting up sensor mode and refresh rate...");
  mlx90640.setMode(MLX90640_CHESS);
  mlx90640.setRefreshRate(MLX90640_8_HZ);
  Serial.println("Sensor configuration complete.");
}

void loop() {}
