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

#define PPR           48 // cambiar esto al numero de pasos por revolucion del motor a usar
#define CNYTransistor A0 //pin del transistor del sensor de color
#define LEDCNY         2 //pin del LED del sensor de color
#define DIST          15 //distancia a la cual se evitan los obstaculos [cm]
#define ULTRAS_TRIG    5 //Pin "Trigger" del sensor ultrasónico
#define ULTRAS_ECHO    4 //Pin "Echo" del sensor ultrasónico
#define COLOR        123 //color a considerar como "azucar"
#define DIAM_RUEDA     6 //diametro de las ruedas [cm]
#define ANCHO_ROBOT   14 //ancho del robot (distancia entre las ruedas) [cm]

SincSteps motores(PPR, 10, 12, 11, 13, PPR, 6, 8, 7, 9);
HormigaExploradora hormiga(&motores, CNYTransistor, LEDCNY, DIST, ULTRAS_TRIG, ULTRAS_ECHO, COLOR, DIAM_RUEDA, ANCHO_ROBOT);

void setup() {
  motores.setSpeed (5);
  hormiga.setEstado(BUSCANDO);
  Serial.begin(9600);
}

void loop() {

  hormiga.trabajar();

  delay(5);
}
