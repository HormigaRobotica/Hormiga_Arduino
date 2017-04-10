/*
* Periodo 2017-1
* U.N.E.F.A. - Núcleo Caracas
* Carrera: Ingeniería Electrónica
* Materia: Introducción a la Robótica
*
* Hormiga (exploradora)
*
* Este programa se encarga de controlar a una hormiga robótica
* que busca un objeto especifico, y al localizarlo,
* avisa a una segunda hormiga de la localización de este.
*/

#include "hormiga.h"

//NOTA: El pin 3 esta reservado para el emisor infrarrojo

#define PPR          48 // cambiar esto al numero de pasos por revolucion del motor a usar
#define DIAM_RUEDA    6 //diametro de las ruedas [cm]
#define ANCHO_ROBOT  14 //ancho del robot (distancia entre las ruedas) [cm]
#define DIST          5 //distancia a la cual se evitan los obstaculos [cm]
#define COLOR        190, 190, 178 //color a considerar como "azucar"

#define ULTRAS_TRIG 12 //Pin "Trigger" del sensor ultrasónico
#define ULTRAS_ECHO 13 //Pin "Echo" del sensor ultrasónico
#define LDR         A0 //pin del transistor del sensor de color
#define RGBLEDS       5,  6,  7 //pines de los LEDs RGB
#define MOTOR1      PPR, 10, 11 //motor1: velocidad, pines
#define MOTOR2      PPR,  8,  9 //motor2: velocidad, pines

SincSteps motores(MOTOR1, MOTOR2);
colorLDR color(RGBLEDS, LDR);
HormigaExploradora hormiga(&motores, DIST, ULTRAS_TRIG, ULTRAS_ECHO, &color, COLOR, DIAM_RUEDA, ANCHO_ROBOT);

void setup() {
  //motores.setSpeed (5);
  hormiga.setEstado(BUSCANDO);
  Serial.begin(9600);
}

void loop() {
  hormiga.desplazar();
  Serial.println(hormiga.getEstado());
  //Serial.println(color.leerColor()[0]);
  //Serial.println(hormiga.Ultrasonico());
}
