/*
* Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier? Electr?ica
* Materia: Introducci? a la Rob?ica
*
* Hormiga (Seguidora)
*
* Este programa se encarga de controlar a una hormiga rob?ica
* que recibe la posici? de un objeto y se desplaza hacia el mismo una vez recibida esta informaci?
*/

#include "hormigaSeguidora.h"

#define PPR       200   //pasos por revolucion para los motores
#define distRuedas 16   //distancia entre las ruedas [cm]
#define diamRueda   6   //diametro de la rueda [cm]

#define LDR       A0 //pin de la LDR del sensor de color
#define trigPin   12 //pin "Trigger" del sensor ultras?ico
#define echoPin   13 //pin "Echo" del sensor ultras?ico
#define enPin      4 //pin para el enable de los motores
#define IRreadpin  3 //pin para el receptor infrarrojo

#define Motor1    PPR, 10, 11 //motor1: pasos por revolucion, pines
#define Motor2    PPR,  8,  9 //motor2: pasos por revolucion, pines
#define RGB_LEDS    5,  6,  7 //pines de los leds del sensor de color

SincSteps motores( Motor1  , Motor2 );
colorLDR  color  ( RGB_LEDS, LDR    );
HormigaSeguidora hormiga(&motores, new IRrecv(IRreadpin), trigPin, echoPin, &color, diamRueda, distRuedas);

void setup() {
	motores.setSpeed (35);
	Serial.begin(9600);

	//establece los pines para la activación/desactivación de los motores
	motores.setEnPin(0, enPin);
	motores.setEnPin(1, enPin);

	hormiga.enableIR();
	hormiga.calHormiga();
}

void loop() {
	hormiga.trabajar();

	delay(100);
}
