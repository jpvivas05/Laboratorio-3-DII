// Universidad del Valle de Guatemala
// BE3029 - Electrónica Digital II
// Juan Pablo Vivas
// 29/07/2025
// Laboratorio 3 Parte2
// MCU: ESP32 dev kit 1.0

#include <Arduino.h>
#include <stdint.h>

// DEFINICIÓN
#define LED1 32
#define LED2 25
#define LED3 14
#define LED4 13

//VARIABLES GLOBALES 
volatile uint8_t cont = 0; // Valor del contador binario
hw_timer_t* timer0 = NULL;  // Puntero al temporizador

// PROTOTIPOS DE FUNCIONES
void IRAM_ATTR ISR_timer(void);
void mostrarDecimal(uint8_t valor);
void apagarTodosLEDs(void);

void setup() {
  // Configuración de pines como salidas
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // Apagar todos los LEDs al inicio
  apagarTodosLEDs();
  mostrarDecimal(cont);

  // Timer 0, prescaler de 80 → 1 tick = 1 µs (80 MHz / 80 = 1 MHz)
  timer0 = timerBegin(0, 80, true); // (número de timer, prescaler, flanco subida)
  timerAttachInterrupt(timer0, &ISR_timer, true); // Asociar ISR
  timerAlarmWrite(timer0, 250000, true); // 250 ms = 250,000 µs
  timerAlarmEnable(timer0); // Habilitar la alarma
}

void loop() {
}

//Configuracion de timer
void IRAM_ATTR ISR_timer() {
  cont++;
  if (cont > 4) cont = 0;
  mostrarDecimal(cont);
}

// Apaga todos los LEDs
void apagarTodosLEDs(void) {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

// Enciende los LEDs según el valor binario
void mostrarDecimal(uint8_t valor) {
  apagarTodosLEDs();
  switch (valor) {
    case 0:
      digitalWrite(LED1, HIGH);
      break;
    case 1:
      digitalWrite(LED2, HIGH);
      break;
    case 2:
      digitalWrite(LED3, HIGH);
      break;
    case 3:
      digitalWrite(LED4, HIGH);
      break;
  }
}
