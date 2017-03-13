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
#define PPR2 200 //pasos por revolucion para motor 2
#define trigPin 12 //pin "Trigger" del sensor ultrasónico
#define echoPin 13 //pin "Echo" del sensor ultrasónico
#define IRreadpin 3 //pin para el receptor infrarrojo
#define pinLDR A0 //pin del LDR del sensor de color
#define dist 15 //distancia a considerar para obstaculos
#define azucarRGB 255,255,255 //color del azucar

//Stepper MP(STEPS, 11,9,10,8);
SincSteps motores(PPR1, 11, 9, 10, 8, PPR2, 4, 6, 5, 7);
HormigaSeguidora hormiga(
  &motores,
  pinLDR, 2, 1, 0,
  dist,
  new IRrecv(IRreadpin),
  trigPin, echoPin,
  azucarRGB);

float resultados;

void setup() {
  Serial.begin(9600);
  hormiga.enableIR();
}

void loop() {
  if (hormiga.chkIR()) {
    Serial.print("resultados: ");
    Serial.println(hormiga.getDatIR(), 4);
  }
  
	//motores.desp(1);

	delay(100);
}
