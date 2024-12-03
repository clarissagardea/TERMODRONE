
//Incluimos la librería para I2C
#include <Wire.h>
 
//Definimos la dirección I2C del MPU
#define MPU 0x68
 
//Definimos los ratios de conversión
#define A_R 16384.0 // Ratio de conversión del acelerómetro
#define G_R 131.0   // Ratio de conversión del giroscopio
 
//Definimos la conversión de radianes a grados
#define RAD_A_DEG 57.295779

//Definimos los límites de la gráfica
#define ANGLE_MIN -90 // Límite mínimo del ángulo
#define ANGLE_MAX 90  // Límite máximo del ángulo

//Offsets individuales para ajustar los valores
float OFFSET_X = -4; // Offset para el eje X
float OFFSET_Y = 3;  // Offset para el eje Y
 
//Variables para almacenar los valores brutos del MPU
int16_t AcX; 
int16_t AcY; 
int16_t AcZ;
int16_t GyX; 
int16_t GyY;
int16_t GyZ;
 
//Declaramos arreglos para los ángulos del acelerómetro y giroscopio
float Acc[3];
float Gy[3];
float Angle[3];

void setup()
{
    //Inicializamos el I2C y el puerto serie
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B); // Registro de configuración del MPU para activarlo
    Wire.write(0);    // Activamos el MPU escribiendo 0 en el registro 0x6B
    Wire.endTransmission(true);
    Serial.begin(9600); // Inicializamos el puerto serie a 9600 baudios
}

void loop()
{
    // Leemos los valores del acelerómetro
    Wire.beginTransmission(MPU);
    Wire.write(0x3B); // Pedimos el registro 0x3B (primer registro del acelerómetro)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); // Leemos 6 registros para los ejes X, Y y Z
    AcX = Wire.read() << 8 | Wire.read(); // Leemos el valor del eje X
    AcY = Wire.read() << 8 | Wire.read(); // Leemos el valor del eje Y
    AcZ = Wire.read() << 8 | Wire.read(); // Leemos el valor del eje Z
 
    // Calculamos los ángulos a partir de los datos del acelerómetro
    Acc[1] = atan(-1 * (AcX / A_R) / sqrt(pow((AcY / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_A_DEG; // Ángulo Y
    Acc[0] = atan((AcY / A_R) / sqrt(pow((AcX / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_A_DEG;      // Ángulo X
 
    // Leemos los valores del giroscopio
    Wire.beginTransmission(MPU);
    Wire.write(0x43); // Pedimos el registro 0x43 (primer registro del giroscopio)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); // Leemos 6 registros para los ejes X, Y y Z
    GyX = Wire.read() << 8 | Wire.read(); // Leemos el valor del eje X
    GyY = Wire.read() << 8 | Wire.read(); // Leemos el valor del eje Y
 
    // Calculamos los valores del giroscopio
    Gy[0] = GyX / G_R; // Velocidad angular en el eje X
    Gy[1] = GyY / G_R; // Velocidad angular en el eje Y
 
    // Aplicamos un filtro complementario para combinar datos del acelerómetro y giroscopio
    Angle[0] = 0.98 * (Angle[0] + Gy[0] * 0.010) + 0.02 * Acc[0]; // Ángulo filtrado en el eje X
    Angle[1] = 0.98 * (Angle[1] + Gy[1] * 0.010) + 0.02 * Acc[1]; // Ángulo filtrado en el eje Y

    // Ajustamos los valores con los offsets individuales
    float angleX = constrain(Angle[0] + OFFSET_X, ANGLE_MIN, ANGLE_MAX); // Ángulo ajustado en el eje X
    float angleY = constrain(Angle[1] + OFFSET_Y, ANGLE_MIN, ANGLE_MAX); // Ángulo ajustado en el eje Y
 
    // Mostramos los valores en el monitor serial con las constantes 90 y -90
    Serial.print(angleX); Serial.print(","); // Valor del ángulo X
    Serial.print(angleY); Serial.print(","); // Valor del ángulo Y
    Serial.print(90); Serial.print(",");     // Constante 90
    Serial.println(-90);                     // Constante -90
}
