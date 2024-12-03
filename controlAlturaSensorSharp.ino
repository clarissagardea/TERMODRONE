// Configuración de pines y constantes
const int pinSensor = 34;  // Pin ADC del ESP32 donde conectas el sensor
const float voltajeObjetivo = 0.90; // Voltaje objetivo que el dron buscará mantener
const int numeroMuestras = 10;     // Número de muestras para promediar

// Variables PID
float kp = 200.0;  // Ganancia proporcional (ajustar según el sistema)
float ki = 20.0;   // Ganancia integral
float kd = 10.0;   // Ganancia derivativa

float errorPrevio = 0.0;    // Error anterior para cálculo derivativo
float errorIntegral = 0.0;  // Acumulador de error para cálculo integral
unsigned long tiempoPrevio = 0; // Para el cálculo de delta tiempo

// Límite para la acumulación del error integral
const float limiteIntegral = 10.0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Leer el valor analógico del sensor
  int valorSensor = leerPromedio(pinSensor, numeroMuestras);

  // Convertir el valor analógico a voltaje
  float voltaje = (valorSensor * 3.3) / 4095.0; // 3.3V y resolución de 12 bits

  // Imprimir el valor del voltaje
  Serial.print("Voltaje del sensor: ");
  Serial.print(voltaje);
  Serial.println(" V");

  // Calcular el error
  float error = voltajeObjetivo - voltaje;

  // Calcular el delta tiempo
  unsigned long tiempoActual = millis();
  float deltaTiempo = (tiempoActual - tiempoPrevio) / 1000.0; // Convertir a segundos
  tiempoPrevio = tiempoActual;

  // Evitar divisiones por 0
  if (deltaTiempo == 0) {
    deltaTiempo = 0.001; // Valor mínimo para evitar problemas
  }

  // Calcular términos PID
  errorIntegral += error * deltaTiempo;  // Integral del error

  // Limitar la acumulación del error integral
  errorIntegral = constrain(errorIntegral, -limiteIntegral, limiteIntegral);

  float errorDerivativo = (error - errorPrevio) / deltaTiempo; // Derivada del error
  errorPrevio = error; // Actualizar el error previo

  // Salida PID
  float salidaPID = (kp * error) + (ki * errorIntegral) + (kd * errorDerivativo);

  // Limitar la salida PID al rango de 0 a 100% para PWM
  int dutyCycle = constrain(salidaPID, 0, 100);

  // Depuración del PID
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" | Integral: ");
  Serial.print(errorIntegral);
  Serial.print(" | Derivativo: ");
  Serial.print(errorDerivativo);
  Serial.print(" | Salida PID: ");
  Serial.println(salidaPID);

  // Imprimir el duty cycle calculado
  Serial.print("Duty cycle motores (PID) = ");
  Serial.print(dutyCycle);
  Serial.println("%");

  // Aplicar el duty cycle a los motores (implementación dependiente del hardware)
  // analogWrite(pinMotor, map(dutyCycle, 0, 100, 0, 255));  // Si usas PWM estándar

  delay(50); // Esperar un breve periodo antes de la siguiente lectura
}

// Función para promediar varias lecturas del ADC
int leerPromedio(int pin, int muestras) {
  long suma = 0;
  for (int i = 0; i < muestras; i++) {
    suma += analogRead(pin);
    delay(10); // Pequeño delay entre lecturas
  }
  return suma / muestras; // Promedio final de las lecturas
}
