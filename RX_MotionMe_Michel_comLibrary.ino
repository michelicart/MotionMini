#include <BTS7960.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int informacao[5];
RF24 radio(2, 10); //(CE,CSN)
const uint64_t pipe = 0x123ABC;

#include "BTS7960.h"

const uint8_t BB_EN = 8;
const uint8_t BB_L_PWM = 6;
const uint8_t BB_R_PWM = 9;

const uint8_t BE_EN = 4;
const uint8_t BE_L_PWM = 3;
const uint8_t BE_R_PWM = 5;

BTS7960 motorBombordo(BB_EN, BB_L_PWM, BB_R_PWM);
BTS7960 motorBoreste(BE_EN, BE_L_PWM, BE_R_PWM);



#define pinAceleradorBombordo 1
#define pinAceleradorBoreste 2

#define margem 50
#define inicioAceleracaoFrente 511 + margem
#define inicioAceleracaoRe 511 - margem
#define inicioDirecaoBombordo 511 - margem
#define inicioDirecaoBoreste 511 + margem




void setup() {
  Serial.begin(9600);           // Inicialização Serial
  radio.begin();                // Inicialização Comunicação NRF
  radio.openReadingPipe(1, pipe); // Modo Receptor
  radio.startListening();


}

void loop() {
  motorBombordo.Enable();
  motorBoreste.Enable();
  int velocidadeMotorBombordo;
  int velocidadeMotorBoreste;
  int boresteMapeadoF;
  int bombordoMapeadoF;
  int boresteMapeadoR;
  int bombordoMapeadoR;

  // Serial.println(informacao[0]);
  velocidadeMotorBombordo = analogRead(pinAceleradorBombordo);
  velocidadeMotorBoreste = analogRead(pinAceleradorBoreste);

  if (velocidadeMotorBombordo <  inicioAceleracaoRe || velocidadeMotorBombordo > inicioAceleracaoFrente || velocidadeMotorBoreste <  inicioAceleracaoRe || velocidadeMotorBoreste > inicioAceleracaoFrente ) {
    bombordoMapeadoF = map(velocidadeMotorBombordo, 511, 1023, 0, 255);
    boresteMapeadoF = map (velocidadeMotorBoreste, 511, 1023, 0, 255);

    bombordoMapeadoR = map(velocidadeMotorBombordo, 511, 0, 0, 255);
    boresteMapeadoR = map (velocidadeMotorBoreste, 511, 0, 0, 255);

    analogRead(pinAceleradorBombordo) > inicioAceleracaoFrente ? motorBombordo.TurnRight(bombordoMapeadoF) : motorBombordo.Nada();
    analogRead(pinAceleradorBombordo) < inicioAceleracaoRe ? motorBombordo.TurnLeft(bombordoMapeadoR) : motorBombordo.Nada();

    analogRead(pinAceleradorBoreste) > inicioAceleracaoFrente ? motorBoreste.TurnRight(boresteMapeadoF) : motorBoreste.Nada();
    analogRead(pinAceleradorBoreste) < inicioAceleracaoRe ? motorBoreste.TurnLeft(boresteMapeadoR) : motorBoreste.Nada();

  }
  else if (velocidadeMotorBombordo >=  inicioAceleracaoRe || velocidadeMotorBombordo <= inicioAceleracaoFrente || velocidadeMotorBoreste >=  inicioAceleracaoRe || velocidadeMotorBoreste <= inicioAceleracaoFrente ) {
    Serial.println("motorparado");
    motorBombordo.Stop();
    motorBoreste.Stop();


  }
}
