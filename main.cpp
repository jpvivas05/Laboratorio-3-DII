//******************************************/
// Universidad del Valle de Guatemala
// BE3029 - Electronica Digital 2
// Juan Pablo Vivas
// 05/02/2025
// Laboratorio 3 DII parte c
// MCU: ESP32 dev kit 1.0
//******************************************/
// Librerias
//******************************************/
#include <Arduino.h>
#include <stdint.h>
//******************************************/
// Definiciones
//******************************************/
#define LED1 13
#define LED2 14
#define LED3 25
#define LED4 32

#define LED5 2
#define LED6 4
#define LED7 5
#define LED8 18

#define LEDalarma 21

#define btn1 33
#define btn2 23

#define touch 15

#define delayBounce 200

//******************************************/
// Prototipos de funciones
//******************************************/
void initbtn(void);
void initleds(void);
void mostrarDecimal(uint16_t valor);
void mostrarDecimal2(uint16_t valor2);

// ISRs
//      Definición de interrupción automática
void IRAM_ATTR ISR_timer(void);
//      Definiciones de interrupciones manuales (botones)
void IRAM_ATTR btn1_ISR(void);
void IRAM_ATTR btn2_ISR(void);

//******************************************/
// Variables globales
//******************************************/

//      Variables de automático
volatile uint8_t cont2 = 0; 
hw_timer_t* timer0 = NULL; 
//      Variables de manual
volatile uint32_t cont1 = 0;

volatile bool btn1Pressed;
volatile uint32_t lastISRbtn1 = 0;

volatile bool btn2Pressed;
volatile uint32_t lastISRbtn2 = 0;

//******************************************/
// ISRs Rutinas de Interrupcion
//******************************************/
void IRAM_ATTR btn1_ISR(void){
  uint32_t tiempoActual1 = millis();
  if (tiempoActual1 - lastISRbtn1 > delayBounce){
    cont1 = (cont1 + 1) % 4;
    btn1Pressed = true;
    lastISRbtn1 = tiempoActual1;
  } 
}

void IRAM_ATTR btn2_ISR(void){
  uint32_t tiempoActual2 = millis();
  if (tiempoActual2 - lastISRbtn2 > delayBounce){
    cont1 = (cont1 - 1 + 4) % 4;
    btn2Pressed = true;
    lastISRbtn2 = tiempoActual2;
  } 
}
//      Contador automático, suma hasta 4
void IRAM_ATTR ISR_timer() {
  cont2 = (cont2 + 1) % 4;
}

//******************************************/
// Configuracion
//******************************************/
void setup() {
  initbtn();
  initleds();

  // Timer 0, prescaler de 80 → 1 tick = 1 µs (80 MHz / 80 = 1 MHz)
  timer0 = timerBegin(0, 80, true); 
  timerAttachInterrupt(timer0, &ISR_timer, true); 
  timerAlarmWrite(timer0, 250000, true); // 250 ms = 250,000 µs
  timerAlarmEnable(timer0); 
}

//******************************************/
// Loop Principal
//******************************************/
void loop() {
  if (btn1Pressed){
    btn1Pressed = false;
    mostrarDecimal(cont1);
  }
  if (btn2Pressed){
    btn2Pressed = false;
    mostrarDecimal(cont1);
  }
  mostrarDecimal2(cont2);
  if (cont1 == cont2){
    cont2=0;    //Reinicia contador automático
    if(digitalRead(LEDalarma)==HIGH){
      digitalWrite(LEDalarma,LOW);
    } else 
    {
      digitalWrite(LEDalarma, HIGH);
    }
  }
  if(touchRead(touch)<40){    //Configuración de pin touch
    cont2=0;    //Reinicia contador automático
  }
}
//******************************************/
// Otras funciones
//******************************************/
void initbtn(void){
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLDOWN);

  attachInterrupt(btn1, &btn1_ISR, FALLING);
  attachInterrupt(btn2, &btn2_ISR, RISING);
}

void initleds(void){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LEDalarma, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
  digitalWrite(LEDalarma, LOW);
}

void mostrarDecimal(uint16_t valor) {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

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

void mostrarDecimal2(uint16_t valor2){
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);

  switch (valor2) {
    case 0:
      digitalWrite(LED5, HIGH);
      break;
    case 1:
      digitalWrite(LED6, HIGH);
      break;
    case 2:
      digitalWrite(LED7, HIGH);
      break;
    case 3:
      digitalWrite(LED8, HIGH);
      break;
  }
}

