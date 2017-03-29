/*
*	Periodo 2017-1
*	U.N.E.F.A. - N�cleo Caracas
*	Carrera: Ingenier�a Electr�nica
*	Materia: Introducci�n a la Rob�tica
*
*	colorLDR.cpp
*
*	Libreria para un sensor de color que hace uso de una LDR y tres LEDs (rojo, verde y azul)
*/

#include "colorLDR.h"

#define NSAMP 5 //npumero de muestras a tomar por medición

colorLDR::colorLDR(byte r, byte g, byte b, byte a){
	pines[0] = r;
	pines[1] = g;
	pines[2] = b;
	pines[3] = a;

	for(int i=0; i<3; i++){
		pinMode(pines[i],OUTPUT);
		cal.blanco[i] = 1023;
		cal.negro [i] = 0;
	}
}

void colorLDR::calibrar(){
	for(int i=0; i < 3 ;i++){
		digitalWrite(i, LOW);
		cal.negro[i] = cal.blanco[i] = 0;
	}
	delay(10);
  
  //calibrar negro
	for(int i=0; i < 3; i++){
		for(int j=0; j < NSAMP; j++)
			cal.negro[i] += analogRead(pines[3]);
		cal.negro[i] /= NSAMP;
	}

	//calibrar blanco
	for(int i=0; i < 3 ;i++){
		digitalWrite(pines[i], HIGH);
		delay(10);

		for(int j=0; j < NSAMP; j++)
			cal.blanco[i] += analogRead(pines[3]);
		cal.blanco[i] /= NSAMP;

		digitalWrite(pines[i], LOW);
	}
}

int* colorLDR::leerColor(){
	for(int i=0; i < 3; i++){
		color[i] = 0;

		digitalWrite(pines[3], HIGH);
		delay(10);

		for(int j=0; j < NSAMP; j++)
			color[i] += analogRead(A0);
		color[i] /= NSAMP;
		color[i] = map(color[i], cal.negro[i], cal.blanco[i], 0, 255);

		digitalWrite(i, LOW);
	}

	return color;
}
