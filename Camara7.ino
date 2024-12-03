#include <Wire.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

const byte MLX90640_address = 0x33;
#define TA_SHIFT 8

static float mlx90640To[768];
paramsMLX90640 mlx90640;

void setup() {
  Wire.begin();
  Wire.setClock(100000);

  Serial.begin(9600);
  while (!Serial);

  Serial.println("MLX90640 IR Array Example");

  if (isConnected() == false) {
    Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
    while (1);
  }
  Serial.println("MLX90640 online!");

  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0) {
    Serial.println("Failed to load system parameters");
  }

  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0) 
    Serial.println("Parameter extraction failed");
}

void loop() {
  uint16_t mlx90640Frame[834];
  int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
  if (status < 0) {
    Serial.print("GetFrame Error: ");
    Serial.println(status);
    delay(1000);
    return; // Salir del loop para evitar procesar datos inválidos
  }

  float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
  float tr = Ta - TA_SHIFT;
  float emissivity = 0.95;

  MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);

  // Enviar datos como CSV
  for (int i = 0; i < 768; i++) {
    // Verificar si el valor es válido
    if (isnan(mlx90640To[i]) || mlx90640To[i] > 1000 || mlx90640To[i] < -100) {
      Serial.print(0); // Reemplazar valores inválidos por 0
    } else {
      Serial.print((int)round(mlx90640To[i])); // Redondear y convertir a entero
    }
    if (i < 767) Serial.print(","); // Separador de valores
  }
  Serial.println(); // Nueva línea al final de cada envío
  delay(1000); // Delay para no saturar el puerto serie
}

// Función para verificar si el sensor MLX90640 está conectado
boolean isConnected() {
  Wire.beginTransmission((uint8_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return false; // El sensor no respondió
  return true;
}