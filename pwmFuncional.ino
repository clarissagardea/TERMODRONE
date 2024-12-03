#include <Arduino.h>

// Pines PWM
#define MOTOR1_PIN 14
#define MOTOR2_PIN 25
#define MOTOR3_PIN 26
#define MOTOR4_PIN 27

// Frecuencia y resolución
#define PWM_FREQUENCY 50 // Frecuencia en Hz
#define PWM_RESOLUTION 8 // Resolución en bits (0-255)

// Duty cycle máximo
#define DUTY_CYCLE_MAX 255

void setup() {
  // Inicializar PWM para cada pin
  ledcSetup(0, PWM_FREQUENCY, PWM_RESOLUTION); // Canal 0 para MOTOR1
  ledcSetup(1, PWM_FREQUENCY, PWM_RESOLUTION); // Canal 1 para MOTOR2
  ledcSetup(2, PWM_FREQUENCY, PWM_RESOLUTION); // Canal 2 para MOTOR3
  ledcSetup(3, PWM_FREQUENCY, PWM_RESOLUTION); // Canal 3 para MOTOR4

  // Adjuntar los canales a los pines
  ledcAttachPin(MOTOR1_PIN, 0);
  ledcAttachPin(MOTOR2_PIN, 1);
  ledcAttachPin(MOTOR3_PIN, 2);
  ledcAttachPin(MOTOR4_PIN, 3);

  // Configurar duty cycle al máximo
  ledcWrite(0, DUTY_CYCLE_MAX); // MOTOR1
  ledcWrite(1, DUTY_CYCLE_MAX); // MOTOR2
  ledcWrite(2, DUTY_CYCLE_MAX); // MOTOR3
  ledcWrite(3, DUTY_CYCLE_MAX); // MOTOR4
}

void loop() {
  // Duty cycle permanece al máximo
}
