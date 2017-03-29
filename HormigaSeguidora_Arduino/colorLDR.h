/*
*	Periodo 2017-1
*	U.N.E.F.A. - N�cleo Caracas
*	Carrera: Ingenier�a Electr�nica
*	Materia: Introducci�n a la Rob�tica
*
*	colorLDR.h
*
*	Libreria para un sensor de color que hace uso de una LDR y tres LEDs (rojo, verde y azul)
*/

#ifndef COLOR_LDR
#define COLOR_LDR

#include <Arduino.h>

class colorLDR{
public:
	colorLDR(byte r, byte g, byte b, byte a);
	void calibrar();
	int* leerColor();

private:
	byte pines[4]; //almacena los pines a usar con el sensor {r,g,b,analogico}
	int  color[3]; //almacena el último color leido
	struct maxCols {   //almacena los colores "blanco y negro" para calibración
		int blanco[3], negro [3];
	}cal;
};

#endif
