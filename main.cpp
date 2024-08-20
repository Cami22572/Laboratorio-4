#include <Arduino.h>
#include <stdint.h>
#include <driver/ledc.h>

//definir canal, frecuencia(HZ) y resolución(bits)
#define canalPWM 0
#define canalrojo 8
#define canalverde 9
#define canalazul 10

#define freqPWM 50
#define resPWM 10
#define servoPWM 23


#define ROJO 32
#define AZUL 14
#define VERDE 26


//posiciones servo diferentes 26, 52, 77, 103, 128
const int servopos[] = {15, 52, 77, 103, 140}; // Pines de los LEDs del contador manual
const int intensidad[] = {0, 200, 500, 800, 1023}; // Pines de los LEDs del contador manual
int intenleds = 0; //para recorrer intensidad

int numpos = 0; // para recorrer servopos
int leds = 0; //numero de leds
int bandera; //bandera para leds3
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// la estructura de los 3 botones
struct Button {
  const uint8_t PIN;
  bool pressed;
  bool laststate;
};


struct Button2 {
  const uint8_t PIN;
  bool pressed;
  bool laststate;
};

struct Button3 {
  const uint8_t PIN;
  bool pressed;
  bool laststate;
};


struct Button4 {
  const uint8_t PIN;
  bool pressed;
  bool laststate;
};


Button button1 = {13, false, false};
Button2 button2 = {25, false, false};
Button3 button3 = {12, false, false};
Button4 button4 = {33, false, false};

// las interrupciones de los 4 botones
void IRAM_ATTR BTN1_ISR(){
  portENTER_CRITICAL_ISR(&mux);
    button1.pressed = true;
  portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR BTN2_ISR(){
  portENTER_CRITICAL_ISR(&mux);
    button2.pressed = true;
  portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR BTN3_ISR(){
  portENTER_CRITICAL_ISR(&mux);
    button3.pressed = true;
  portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR BTN4_ISR(){
  portENTER_CRITICAL_ISR(&mux);
    button4.pressed = true;
  portEXIT_CRITICAL_ISR(&mux);
}



void initPWM(void);
void initrojo(void);
void initverde(void);
void initazul(void);
void parte3(void);


void setup() {
  
  initPWM();
  initrojo();
  initverde();
  initazul();

  Serial.begin(115200);
  //Paso 2
//  pinMode(ROJO, OUTPUT);
 // pinMode(AZUL, OUTPUT);
 // pinMode(VERDE, OUTPUT);

  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(button2.PIN, INPUT_PULLUP);
  pinMode(button3.PIN, INPUT_PULLUP);
  pinMode(button4.PIN, INPUT_PULLUP);


  attachInterrupt(button1.PIN,  BTN1_ISR, RISING);
  attachInterrupt(button2.PIN, BTN2_ISR, RISING);
  attachInterrupt(button3.PIN, BTN3_ISR, RISING);
  attachInterrupt(button4.PIN, BTN4_ISR, RISING);
   
}



void loop() {

// condición para saber si se presionó el botón y aumentar el contador
  if(button1.pressed != button1.laststate){
    if(button1.pressed){
      
      if (numpos > 4){
        numpos = 0;
      }
      Serial.printf("el numpos %u \n", numpos);
      Serial.printf("Se aumentó la posición a %u", servopos[numpos]);
      ledcWrite(canalPWM, servopos[numpos]);
      numpos ++; 
    }
    button1.pressed = false;
  } 

  // condición para saber si se presionó el botón y decrementar el contador
  if(button2.pressed != button2.laststate){
    if(button2.pressed){
        if (numpos < 0){
        numpos=4;
      }
      
      

      
      Serial.printf("el numpos %u \n", numpos);
      Serial.printf("Se redujo la posición a %u", servopos[numpos]);
      ledcWrite(canalPWM, servopos[numpos]);
      numpos --;
    }
    button2.pressed = false;
  }
//condición para resetear el contador del timer
    if(button3.pressed != button3.laststate){
    if(button3.pressed){
      bandera = 0;
      leds++;

      Serial.printf("La led escogida es: %u", leds);
      if(leds > 2){
        if (leds == 3){
          bandera = 1;
          Serial.printf("entre a la bandera: %u", numpos);
        }
        leds=-1;
      }

 
      button3.pressed = false;
    }
  }

  if(button4.pressed != button4.laststate){
    if(button4.pressed){
      intenleds++;
      Serial.printf("la intensidad de la led es %u", intensidad[intenleds]);
      switch(leds){
        case 0:
          ledcWrite(canalrojo, intensidad[intenleds]);
          
      Serial.printf("entra rojo %u \n", intensidad[intenleds]);
          break;
        case 1:
          ledcWrite(canalverde, intensidad[intenleds]);
          
      Serial.printf("entra verde %u \n", intensidad[intenleds]);
          break;
        case 2:
          ledcWrite(canalazul, intensidad[intenleds]);
          
      Serial.printf("entra azul %u \n", intensidad[intenleds]);
          break;
      }
      if(intenleds >= 4){
        intenleds = -1;
      }
      button4.pressed = false;
    }
  }


if(bandera == 1){
        //const int servopos[] = {51, 64, 77, 90, 102}; // Pines de los LEDs del contador manual

        if (numpos == 0 ){
            ledcWrite(canalrojo, 1023);
            ledcWrite(canalverde, 0);
            ledcWrite(canalazul, 0);

            //Serial.println("enciendo rojo");

        }
        if (numpos == 1 ){
            ledcWrite(canalrojo, 0);
            ledcWrite(canalverde, 1023);
            ledcWrite(canalazul, 0);

            //Serial.println("enciendo rojo");

        }
        if (numpos == 2 ){
            ledcWrite(canalrojo, 0);
            ledcWrite(canalverde, 1023);
            ledcWrite(canalazul, 0);

            //Serial.println("enciendo rojo");

        }
        if (numpos == 3 ){
            ledcWrite(canalrojo, 0);
            ledcWrite(canalverde, 0);
            ledcWrite(canalazul, 1023);

            //Serial.println("enciendo rojo");

        }
        if (numpos == 4){
            ledcWrite(canalrojo, 0);
            ledcWrite(canalverde, 0);
            ledcWrite(canalazul, 1023);

            //Serial.println("enciendo rojo");

        }
 
      }

//cosas para antirebote
  button1.laststate = button1.pressed;
  button2.laststate = button2.pressed;
  button3.laststate = button3.pressed;
  button4.laststate = button4.pressed;  

  // Pequeño retardo para evitar el rebote del botón
  delay(50);


}




void initPWM(void){
  ledcSetup(canalPWM, freqPWM, resPWM);
  ledcAttachPin(servoPWM, canalPWM);
  ledcWrite(canalPWM, 0);
}

void initrojo(void){
  ledcSetup(canalrojo, freqPWM, resPWM);
  ledcAttachPin(ROJO, canalrojo);
  ledcWrite(canalrojo, 0);
}

void initverde(void){
  ledcSetup(canalverde, freqPWM, resPWM);
  ledcAttachPin(VERDE, canalverde);
  ledcWrite(canalverde, 0);
}

void initazul(void){
  ledcSetup(canalazul, freqPWM, resPWM);
  ledcAttachPin(AZUL, canalazul);
  ledcWrite(canalazul, 0);
}

void parte3(void){
  

}