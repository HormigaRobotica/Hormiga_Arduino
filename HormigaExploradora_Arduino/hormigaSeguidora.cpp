/*
*  Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier?a Electr?nica
* Materia: Introducci?n a la Rob?tica
*
* hormigaSeguidora.cpp
*
* Libreria para el funcionamiento de las "hormigas seguidoras" roboticas en este proyecto
*/

#include "hormigaSeguidora.h"

//Constructores: Hormiga seguidora ******************************************
HormigaSeguidora::HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
	float     distObstaculo, //distancia a la cual se evitan los obstaculos
	IRrecv*   IRRead       , //lector del sensor infrarrojo
	byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
	byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
	colorLDR* RGB          , //pin de la LDR del sensor de color
	byte      colorR       , byte  colorG    , byte colorB, //color a considerar como "azucar"
	float     diamRueda    , float anchoRobot) : //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, RGB, colorR, colorG, colorB, diamRueda, anchoRobot),
	IRRead (IRRead),
	IRRes  (new decode_results)
{
	setEstado(ESPERANDO);
}

HormigaSeguidora::HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
	IRrecv*   IRRead       , //lector del sensor infrarrojo
	byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
	byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
	colorLDR* RGB          , //pin de la LDR del sensor de color
	float     diamRueda    , float anchoRobot) : //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, RGB, diamRueda, anchoRobot),
	IRRead (IRRead),
	IRRes  (new decode_results)
{
	setEstado(ESPERANDO);
}
//Fin: Constructores: Hormiga seguidora *************************************

bool HormigaSeguidora::chkIR(){
	if (IRRead->decode(IRRes)){
		datosIR = *reinterpret_cast<float*>(&IRRes->value);

		setVectorAng(datosIR);

		IRRead->resume();

		return true;
	}
	return false;
}

float HormigaSeguidora::getDatIR(){ return datosIR; }
void  HormigaSeguidora::enableIR(){ IRRead->enableIRIn(); }

//realiza el siguiente trabajo pendiente (basado en el estado)
void HormigaSeguidora::trabajar(){
	switch (getEstado()){
		case ESPERANDO:
			if(chkIR()){
				setUltAng(getVector()[1]);
				setEstado(SIGUIENDO);
			}
			break;
		case SIGUIENDO:
			seguir();
			break;
		case ENCONTRADA:
			retornar();
			setEstado(ESPERANDO);
			break;
	};
}
