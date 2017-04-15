/*
*	Periodo 2017-1
*	U.N.E.F.A. - N?cleo Caracas
*	Carrera: Ingenier?a Electr?nica
*	Materia: Introducci?n a la Rob?tica
*
*	colorLDR.h
*
*	Libreria para un sensor de color que hace uso de una LDR y tres LEDs (rojo, verde y azul)
*/

#ifndef COLOR_LDR
#define COLOR_LDR

#include <Arduino.h>

//DEBUG SNIPPETS ******************************************

#define PRINT_COLOR(c) { \
		Serial.print("r: "    ); Serial.print  (c[0]); \
		Serial.print("    g: "); Serial.print  (c[1]); \
		Serial.print("    b: "); Serial.println(c[2]); \
	}

//Fin: DEBUG SNIPPETS *************************************

class colorLDR{
public:
	colorLDR(byte r, byte g, byte b, byte a);
	void calibrar();
	int* leerColor();
	bool compColor(int* a, int* b);
	bool compColor(int* a, int* b, byte tol);

private:
	byte   pines[4]; //almacena los pines a usar con el sensor {r,g,b,analogico}
	int    color[3]; //almacena el ultimo color leido
	struct maxCols { //almacena los colores "blanco y negro" para calibraci?
		int blanco[3], negro [3];
	}cal;
};

#endif
