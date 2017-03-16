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
		void calColor(); //calibra el color a reconocer como azucar
		void toleranciaColor(int colorTol); //establece la tolerancia para el sensor de color (por defecto = +-2)
	protected:
		Hormiga(SincSteps* despl,
			byte trigPin,
			byte echoPin,
			int dist,
			byte pinTransistorCNY, byte pinLEDCNY,
			int color);
	private:
		SincSteps* despl; //puntero a la instancia para la sinsronización de los motores
		byte trigPin; //Pin "Trigger" del sensor ultrasónico
		byte echoPin; //Pin "Echo" del sensor ultrasónico
		int dist; //distancia a la cual se evitan los obstaculos
		byte pinTransistorCNY; //pin para transistor del sensor de color
		byte pinLEDCNY; //pin para LED del sensor de color
		int color; //color a considerar como "azucar"
		int colorTol; //tolerancia del color
};
#endif

//***************************************************************************************************

#ifndef HORMIGA_S_H
#define HORMIGA_S_H
class HormigaSeguidora : public Hormiga {
	public:
		HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
			byte pinTransistorCNY, byte pinLEDCNY, //pines del transistor y el LED del sensor de color
			int distObstaculo, //distancia a la cual se evitan los obstaculos
			IRrecv* IRRead, //lector del sensor infrarrojo
			byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
			byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
			int color); //color a considerar como "azucar"

		bool chkIR(); //revisa si el infrarrojo recibio datos
		float getDatIR(); //retorna los datos recibidos por infrarrojo
		void enableIR();
	private:
		IRrecv* IRRead;
		decode_results* IRRes;
		float datosIR;
};
#endif

//***************************************************************************************************

#ifndef HORMIGA_E_H
#define HORMIGA_E_H
class HormigaExploradora : public Hormiga {
	public:
		HormigaExploradora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
			byte pinTransistorCNY, byte pinLEDCNY, //pines del transistor y el LED del sensor de color
			int distObstaculo, //distancia a la cual se evitan los obstaculos
			byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
			byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
			int color); //color a considerar como "azucar"
		
    	void enviarIR(float datos); //envía datos usando el emisor infrarrojo
	private:
		IRsend irsend;
};

#endif
