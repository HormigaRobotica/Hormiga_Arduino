/*
*  Periodo 2017-1
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

#include <IRremote.h>
#include <Stepper.h>

const int pasosPorRevolucion = 48; // cambiar esto al numero de pasos por revolucion del motor a usar

// inicializa la libreria stepper
// Stepper( steps, pin1, pin2 [, pin3, pin4] );

Stepper motor1(pasosPorRevolucion, 8, 9);
Stepper motor2(pasosPorRevolucion, 10, 11);

// numero de pasos que representan un dsplazamiento
int Desplazamiento = 5;
int stepCount = 0;
int IRreadpin = 5;

IRrecv irrecv(IRreadpin);
decode_results results;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
  
	// put your main code here, to run repeatedly:
	motor1.step(1);
  motor2.step(-1);
  
	Serial.print("N pasos:");
	Serial.println(stepCount);
	stepCount++;
	delay(500);
}
