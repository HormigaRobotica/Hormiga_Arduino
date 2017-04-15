/*
*  Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier?a Electr?nica
* Materia: Introducci?n a la Rob?tica
*
* hormigaSeguidora.h
*
* Libreria para el funcionamiento de las "hormigas seguidoras" roboticas en este proyecto
*/

#include "hormiga.h"

#ifndef HORMIGA_S_H
#define HORMIGA_S_H
class HormigaSeguidora : public Hormiga {
	public:
		HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
			float     distObstaculo, //distancia a la cual se evitan los obstaculos
			IRrecv*   IRRead       , //lector del sensor infrarrojo
			byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
			byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
			colorLDR* RGB          , //pin de la LDR del sensor de color
			byte      colorR       , byte  colorG    , byte colorB, //color a considerar como "azucar"
			float     diamRueda    , float anchoRobot); //diametro de la rueda y ancho del robot

		HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
			IRrecv*   IRRead       , //lector del sensor infrarrojo
			byte      UltrasTrigPin, //pin "Trigger" del sensor ultras?ico
			byte      UltrasEchoPin, //pin "Echo" del sensor ultras?ico
			colorLDR* RGB          , //pin de la LDR del sensor de color
			float     diamRueda    , float anchoRobot); //diametro de la rueda y ancho del robot

		bool  chkIR   (); //revisa si el infrarrojo recibio datos
		float getDatIR(); //retorna los datos recibidos por infrarrojo
		void  enableIR(); //activa la funcionalidad del infrarrojo
		void  trabajar(); //realiza el siguiente trabajo pendiente (basado en el estado)
	private:
		IRrecv*         IRRead;
		decode_results* IRRes;
		float           datosIR;
};
#endif
