#include <BTS7960.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "BTS7960.h"

RF24 radio(2, 10); //(CE,CSN)
const uint64_t pipe = 0x123ABC;

//Define o pino utilizado para ligar o motor de Bombordo
const uint8_t BB_EN = 8;
//Define o pino utilizado para passar a rotação do motor de Bombordo no sendo anti horário
const uint8_t BB_L_PWM = 6;
//Define o pino utilizado para passar a rotação do motor de Bombordo no sendo horário
const uint8_t BB_R_PWM = 9;

//Define o pino utilizado para ligar o motor de Boreste
const uint8_t BE_EN = 4;
//Define o pino utilizado para passar a rotação do motor de Boreste no sendo anti horário
const uint8_t BE_L_PWM = 3;
//Define o pino utilizado para passar a rotação do motor de Boreste no sendo horário
const uint8_t BE_R_PWM = 5;

BTS7960 motorBombordo(BB_EN, BB_L_PWM, BB_R_PWM);
BTS7960 motorBoreste(BE_EN, BE_L_PWM, BE_R_PWM);



#define pinAceleradorBombordo 1
#define pinAceleradorBoreste 2

//Parâmetros para calibrar os aceleradores
#define margem 50
#define inicioAceleracaoFrente 511 + margem
#define inicioAceleracaoRe 511 - margem
#define inicioDirecaoBombordo 511 - margem
#define inicioDirecaoBoreste 511 + margem




void setup() {
  Serial.begin(9600);           // Inicialização Serial

}

void loop() {
//  Ativa os motores utilizando a library BTS
  motorBombordo.Enable();
  motorBoreste.Enable();
  
//  Valor passado pelos potênciometros
  int velocidadeMotorBombordo;
  int velocidadeMotorBoreste;
//  Conversão dos valores dos potenciometros para 0 - 255 separados pelos sentidos
  int boresteMapeadoF;
  int bombordoMapeadoF;
  int boresteMapeadoR;
  int bombordoMapeadoR;


  velocidadeMotorBombordo = analogRead(pinAceleradorBombordo);
  velocidadeMotorBoreste = analogRead(pinAceleradorBoreste);
  
//Analisa se algum dos aceleradores está em alguma posição fora da posição parado
  if (velocidadeMotorBombordo <  inicioAceleracaoRe || velocidadeMotorBombordo > inicioAceleracaoFrente || velocidadeMotorBoreste <  inicioAceleracaoRe || velocidadeMotorBoreste > inicioAceleracaoFrente ) {

//   Converte os valores dos potenciometros para o padrão lido pela ponte H
    bombordoMapeadoF = map(velocidadeMotorBombordo, 511, 1023, 0, 255);
    boresteMapeadoF = map (velocidadeMotorBoreste, 511, 1023, 0, 255);
    bombordoMapeadoR = map(velocidadeMotorBombordo, 511, 0, 0, 255);
    boresteMapeadoR = map (velocidadeMotorBoreste, 511, 0, 0, 255);

//Define o sentido e velocidade da aceleracão do motor de bombordo
    analogRead(pinAceleradorBombordo) > inicioAceleracaoFrente ? motorBombordo.TurnRight(bombordoMapeadoF) : motorBombordo.Nada();
    analogRead(pinAceleradorBombordo) < inicioAceleracaoRe ? motorBombordo.TurnLeft(bombordoMapeadoR) : motorBombordo.Nada();

//Define o sentido e velocidade da aceleracão do motor de boreste
    analogRead(pinAceleradorBoreste) > inicioAceleracaoFrente ? motorBoreste.TurnRight(boresteMapeadoF) : motorBoreste.Nada();
    analogRead(pinAceleradorBoreste) < inicioAceleracaoRe ? motorBoreste.TurnLeft(boresteMapeadoR) : motorBoreste.Nada();

  }
//  Checa se os dois aceleradores estão parados e desliga os motores
  else if (velocidadeMotorBombordo >=  inicioAceleracaoRe || velocidadeMotorBombordo <= inicioAceleracaoFrente || velocidadeMotorBoreste >=  inicioAceleracaoRe || velocidadeMotorBoreste <= inicioAceleracaoFrente ) {
    Serial.println("motorparado");
    motorBombordo.Stop();
    motorBoreste.Stop();


  }
}
