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

#include "hormigaExploradora.h"

//NOTA: El pin 3 esta reservado para el emisor infrarrojo

#define PPR         48 // cambiar esto al numero de pasos por revolucion del motor a usar
#define ANCHO_ROBOT 10.5 //ancho del robot (distancia entre las ruedas) [cm]
#define DIAM_RUEDA   5.8 //diametro de las ruedas [cm]

#define LDR         A0 //pin del transistor del sensor de color
#define ULTRAS_ECHO 13 //Pin "Echo" del sensor ultrasónico
#define ULTRAS_TRIG 12 //Pin "Trigger" del sensor ultrasónico
#define INI_BTN      4 //pin de inicio de busqueda

#define MOTOR1      PPR, 10, 11 //motor1: pasos por revolucion, pines
#define MOTOR2      PPR,  8,  9 //motor2: pasos por revolucion, pines
#define RGBLEDS       5,  6,  7 //pines de los LEDs RGB

SincSteps motores( MOTOR1 , MOTOR2 );
colorLDR  color  ( RGBLEDS, LDR    );
HormigaExploradora hormiga(&motores, ULTRAS_TRIG, ULTRAS_ECHO, INI_BTN, &color, DIAM_RUEDA, ANCHO_ROBOT);

void setup() {
	//motores.setSpeed (5);
	Serial.begin(9600);

	hormiga.calHormiga();
	hormiga.setEstado(ESPERANDO);
}

void loop() {
	
	hormiga.trabajar();
	Serial.println(hormiga.getEstado());
}
