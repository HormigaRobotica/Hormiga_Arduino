/*
*	Periodo 2017-1
*	U.N.E.F.A. - N�cleo Caracas
*	Carrera: Ingenier�a Electr�nica
*	Materia: Introducci�n a la Rob�tica
*
*	hormiga.h
*
*	Libreria para el funcionamiento estandar de las "hormigas" roboticas en este proyecto
*/

#ifndef HORMIGA_H
#define HORMIGA_H

#include <IRremote.h>
#include "sincSteppers.h"

class Hormiga {
	public:
		void iniBusqueda();
		float Ultrasonico();
	protected:
		Hormiga(SincSteps* despl,
			byte trigPin,
			byte echoPin,
			int dist,
			byte pinLDR,
			byte rgbLEDPin1, byte rgbLEDPin2, byte rgbLEDPin3,
			int r, int g, int b);
	private:
		SincSteps* despl; //puntero a la instancia para la sinsronización de los motores
		byte trigPin; //Pin "Trigger" del sensor ultrasónico
		byte echoPin; //Pin "Echo" del sensor ultrasónico
		int dist; //distancia a la cual se evitan los obstaculos
		byte pinLDR; //pin para LDR del sensor de color
		byte rgbLEDsPins[3]; //pines para los LEDs del sensor de color
		int rgb[3]; //color a considerar como "azucar"
};

class HormigaSeguidora : public Hormiga {
	public:
		HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
			byte LDRPin, //pin para LDR del sensor de color
			byte rgbLEDPin1, byte rgbLEDPin2, byte rgbLEDPin3, //pines para los LEDs del sensor de color
			int distObstaculo, //distancia a la cual se evitan los obstaculos
			IRrecv* IRRead, //lector del sensor infrarrojo
			byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
			byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
			int r, int g, int b); //color a considerar como "azucar"

		bool chkIR(); //revisa si el infrarrojo recibio datos
		float getDatIR(); //retorna los datos recibidos por infrarrojo
    void enableIR();
	private:
		IRrecv* IRRead;
		decode_results* IRRes;
		float datosIR;
};

#endif
