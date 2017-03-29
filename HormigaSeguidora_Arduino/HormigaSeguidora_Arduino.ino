/*
* Periodo 2017-1
* U.N.E.F.A. - Núcleo Caracas
* Carrera: Ingeniería Electrónica
* Materia: Introducción a la Robótica
*
* Hormiga (Seguidora)
*
* Este programa se encarga de controlar a una hormiga robótica
* que recibe la posición de un objeto y se desplaza hacia el mismo una vez recibida esta información
*/

#include "hormiga.h"

#define PPR1 48 //pasos por revolucion para motor 1
#define PPR2 48 //pasos por revolucion para motor 2
#define trigPin 12 //pin "Trigger" del sensor ultrasónico
#define echoPin 13 //pin "Echo" del sensor ultrasónico
#define IRreadpin 3 //pin para el receptor infrarrojo
#define transistorCNY A0 //pin del transistor del sensor de color
#define LED_CNY 2 //pin del LED infrarrojo del sensor de color
#define dist 15 //distancia a considerar para obstaculos
#define azucarRGB 255.0 //color del azucar
#define diamRueda 6 //diametro de la rueda [cm]
#define distRuedas 14 //distancia entre las ruedas [cm]

//Stepper MP(STEPS, 11,9,10,8);
SincSteps motores(PPR1, 10, 11, PPR2, 8, 9);
HormigaSeguidora hormiga(&motores, transistorCNY, LED_CNY, dist, new IRrecv(IRreadpin), trigPin, echoPin, azucarRGB, diamRueda, distRuedas);

float resultados;
void setup() {
  Serial.begin(9600);
  hormiga.enableIR();
  hormiga.setVector(30, 0.7854);
  hormiga.setUltAng(0.7854);
}

void loop() {
  hormiga.seguir();
  //motores.Step(0, 15);

  Serial.println(hormiga.getEstado());

  delay(500);
}
