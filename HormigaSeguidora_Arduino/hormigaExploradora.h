/*
*  Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier?a Electr?nica
* Materia: Introducci?n a la Rob?tica
*
* hormigaExploradora.h
*
* Libreria para el funcionamiento de las "hormigas exploradoras" roboticas en este proyecto
*/

#include "hormiga.h"

#ifndef HORMIGA_E_H
#define HORMIGA_E_H
class HormigaExploradora : public Hormiga {
	public:
		HormigaExploradora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
			float     distObstaculo, //distancia a la cual se evitan los obstaculos
			byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
			byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
			byte      iniBtn       , //pin de inicio de busqueda
			colorLDR* RGB          , //pin de la LDR del sensor de color
			byte      colorR       , byte  colorG, byte colorB, //color a considerar como "azucar"
			float     diamRueda    , float anchoRobot); //diametro de la rueda y ancho del robot

		HormigaExploradora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
			byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
			byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
			byte      iniBtn       , //pin de inicio de busqueda
			colorLDR* RGB          , //pin de la LDR del sensor de color
			float     diamRueda    , float anchoRobot); //diametro de la rueda y ancho del robot

		void enviarIR   (float datos); //env? datos usando el emisor infrarrojo
		void iniBusqueda(); //inicia la busqueda del azucar
		void trabajar   (); //realiza el siguiente trabajo pendiente (basado en el estado)
	private:
		IRsend irsend;
		byte   iniBtn;
};

#endif