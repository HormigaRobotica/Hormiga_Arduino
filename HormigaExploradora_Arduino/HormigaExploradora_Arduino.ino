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
#include <IRremote.h>
#include "hormiga.h"

#define PPR 48 // cambiar esto al numero de pasos por revolucion del motor a usar

SincSteps motores(PPR, 11, 9, 10, 8, PPR, 4, 6, 5, 7);
IRsend irsend;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println("enviando datos");
  for (int i = 0; i < 3; i++) {
    float f = 7.155;
    irsend.sendSony(*reinterpret_cast<unsigned long*>(&f), 32); // Sony TV power code
    delay(100);
  }
  Serial.println("datos enviados");
  
  motores.desp(1); //mueve los motores 1 desplazamiento

  delay(1000);
}
