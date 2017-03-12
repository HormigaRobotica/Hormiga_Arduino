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

#include <IRremote.h>
#include "hormiga.h"

#define PPR 48 // cambiar esto al numero de pasos por revolucion del motor a usar
#define trigPin 12
#define echoPin 13
#define IRreadpin 5


SincSteps motores(PPR, 11, 9, 10, 8, PPR, 4, 6, 5, 7);

//Stepper MP(STEPS, 11,9,10,8);

float resultados;

IRrecv irrecv(IRreadpin);
decode_results results;

void setup() {
	// Código a ejecutarse solo una vez
	Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    resultados = *reinterpret_cast<float*>(&results.value);
    Serial.print("resultados: ");
    Serial.println(resultados, 4);
    irrecv.resume();
  }
  
	motores.desp(1); //mueve los motores 1 desplazamiento

	delay(500);
}
