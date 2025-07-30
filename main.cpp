// Universidad del Valle de Guatemala
// BE3029 - Electrónica Digital II
// Juan Pablo Vivas
// 29/07/2025
// Laboratorio 3
// MCU: ESP32 dev kit 1.0

// Librerías
#include <Arduino.h>
#include <stdint.h>


// Definiciones
#define LED1 32
#define LED2 25
#define LED3 14
#define LED4 13
#define B1 18 // Botón para aumentar
#define B2 21 // Botón para disminuir
#define B3 23 // Botón para cambiar de modo

//Prototipos de funciones
void actualizarLeds();
void mostrarDecimal(int valor);
void initBoton(void);
void IRAM_ATTR btn1_ISR(void);
void initTMR0(void);

//Variables globales
//Instanciar timer
hw_timer_t * Timer0_Cfg = NULL;
volatile u_int32_t cont;
volatile bool btn1Pressed;
volatile uint32_t lastISRbtn1 = 0;

//Rutinas de interrupción
void IRAM_ATTR TMR0_ISR(void){
  
}
void IRAM_ATTR btn1_ISR(void){
  uint32_t tiempoActual=millis();
  if(tiempoActual-lastISRbtn1>50){
    btn1Pressed = true;
    cont++;
    lastISRbtn1=tiempoActual;
  }
  unsigned long tiempoActual = millis();

  // Lógica para el botón B1
  bool lecturaB1 = digitalRead(B1);
  bool lecturaB2 = digitalRead(B2);

  // Detectar un flanco de subida
  if (lecturaB1 == HIGH && ultimoEstadoB1 == LOW) {
    if ((tiempoActual - tiempoUltimaPulsacionB1) > DEBOUNCE_DELAY) {
        cont = (cont + 1) % 4;
      tiempoUltimaPulsacionB1 = tiempoActual; // Guarda el tiempo de esta pulsación válida
    }
  }
  ultimoEstadoB1 = lecturaB1; // Actualiza el estado anterior del botón B1

  //Detectar un flanco de bajada
  if (lecturaB2 == LOW && ultimoEstadoB2 == HIGH) {
    if ((tiempoActual - tiempoUltimaPulsacionB2) > DEBOUNCE_DELAY) {
        cont = (cont - 1 + 4) % 4;
      tiempoUltimaPulsacionB2 = tiempoActual; // Guarda el tiempo de esta pulsación válida
    }
  }
  ultimoEstadoB2 = lecturaB2; // Actualiza el estado anterior del botón B2
}
// Estado anterior de cada botón para detectar el flanco de subida/bajada
bool ultimoEstadoB1 = LOW;
bool ultimoEstadoB2 = HIGH;

// Tiempos para el debounce de cada botón
unsigned long tiempoUltimaPulsacionB1 = 0;
unsigned long tiempoUltimaPulsacionB2 = 0;
const unsigned long DEBOUNCE_DELAY = 250; // Retardo de debounce en milisegundos.

//Paso 1. Instanciar TIMER 0
hw_timer_t *Timer0_Cfg = NULL;

// Configuración
void setup() {
  // Configuración de pines de LEDs como salidas
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  //Configuraciones para timer 0
  initTMR0();
  initBoton(); //configurar boton
  cont =0;
}
// Loop infinito
void loop() {
  if(btn1Pressed){
  btn1Pressed=false;
 }
 if(btn2Pressed){
  btn2Pressed=false;
 }
}

// Funciones Auxiliares
void initBoton(void){
  //entrada con pullup
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLDOWN);

  //configurar ISR
  attachInterrupt(btn1,&btn1_ISR, FALLING);
  attachInterrupt(btn2,&btn2_ISR, FALLING);
}
void initTMR0(void){
  //Paso 2: Configurar timer 0
  Timer0_Cfg = timerBegin(0, 80, true); //Cuando es temporizador no importa el flag (True)
  //Paso 3: configurar handler ISR
  timerAttachInterrupt(Timer0_Cfg, &TMR0_ISR, true);
  //Paso 4: Configurar alarma (Timer Ticks)
  timerAlarmWrite(Timer0_Cfg, 500000, true);
  //Paso 5: Iniciar la alarma
  timerAlarmEnable(Timer0_Cfg);
}

void mostrarDecimal(int valor) {
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