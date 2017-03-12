/*
* Periodo 2017-1
* U.N.E.F.A. - N�cleo Caracas
* Carrera: Ingenier�a Electr�nica
* Materia: Introducci�n a la Rob�tica
*
* sensor.cpp
*
* Libreria para los sensores de distancia y color
*/

#include "sensor.h"

//mide la distancia entre el ultrasonico y un obstáculo
float Ultrasonico(byte trigPin, byte echoPin){
	//envia la onda de sonido
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	//retorna la distancia a la cual se encuentra el sensor ultrasonico
	return (((pulseIn(echoPin, HIGH) / 2) * 0.03438) -1);
}

