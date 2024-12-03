#include <Wire.h>
#include <WiFi.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

// WiFi Configuration
const char* ssid = "T";
const char* password = "12345678";

WiFiServer server(80);

const byte MLX90640_address = 0x33;
#define TA_SHIFT 8

static float mlx90640To[768];
paramsMLX90640 mlx90640;

// Function Prototypes
void setupWiFi(void *parameter);
void setupMLX90640();

// FreeRTOS Task Handles
TaskHandle_t WiFiTaskHandle;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize I2C
  Wire.begin();
  Wire.setClock(50000); // Reduce speed for stability

  // Setup MLX90640
  setupMLX90640();

  // Start WiFi setup in a separate task
  xTaskCreatePinnedToCore(setupWiFi, "WiFiTask", 10000, NULL, 1, &WiFiTaskHandle, 0);
}

void loop() {
  // Get data from the sensor
  uint16_t mlx90640Frame[834];
  int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
  if (status < 0) {
    Serial.print("GetFrame Error: ");
    Serial.println(status);
    delay(1000);
    return;
  }

  float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
  float tr = Ta - TA_SHIFT;
  float emissivity = 0.95;

  MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);

  // Serve data over WiFi
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();

    // Send sensor data as CSV
    for (int i = 0; i < 768; i++) {
      if (isnan(mlx90640To[i]) || mlx90640To[i] > 1000 || mlx90640To[i] < -100) {
        client.print(0);
      } else {
        client.print((int)round(mlx90640To[i]));
      }
      if (i < 767) client.print(",");
    }
    client.println(); // End of data
    client.stop();
    Serial.println("Data sent to client.");
  }

  delay(1000); // Delay between reads
}

void setupMLX90640() {
  Serial.println("Initializing MLX90640...");

  if (!isConnected()) {
    Serial.println("MLX90640 not detected. Check wiring.");
    while (1);
  }

  uint16_t eeMLX90640[832];
  Serial.println("Reading EEPROM...");
  int status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0) {
    Serial.println("Failed to load EEPROM data.");
    while (1);
  }

  delay(200); // Ensure the sensor is ready

  Serial.println("Extracting parameters...");
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0) {
    Serial.println("Failed to extract parameters.");
    while (1);
  }

  Serial.println("MLX90640 ready.");
}

void setupWiFi(void *parameter) {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start HTTP server
  vTaskDelete(NULL); // End this task
}

boolean isConnected() {
  Wire.beginTransmission(MLX90640_address);
  return (Wire.endTransmission() == 0);
}
