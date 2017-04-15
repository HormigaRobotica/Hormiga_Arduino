/*
*  Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier?a Electr?nica
* Materia: Introducci?n a la Rob?tica
*
* hormigaExploradora.cpp
*
* Libreria para el funcionamiento de las "hormigas exploradoras" roboticas en este proyecto
*/

#include "hormigaExploradora.h"

//Constructores: Hormiga exploradora ******************************************
HormigaExploradora::HormigaExploradora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
	float     distObstaculo, //distancia a la cual se evitan los obstaculos
	byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
	byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
	byte      iniBtn       , //pin de inicio de busqueda
	colorLDR* RGB          , //pin de la LDR del sensor de color
	byte      colorR       , byte  colorG    , byte colorB, //color a considerar como "azucar"
	float     diamRueda    , float anchoRobot) : //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, RGB, colorR, colorG, colorB, diamRueda, anchoRobot),
	iniBtn(iniBtn)
{
	setEstado(ESPERANDO);
	pinMode(iniBtn, INPUT_PULLUP);
}

HormigaExploradora::HormigaExploradora(SincSteps* sinc, //puntero a la instancia para la sinsronizaci? de los motores
	byte      UltrasTrigPin, //Pin "Trigger" del sensor ultras?ico
	byte      UltrasEchoPin, //Pin "Echo" del sensor ultras?ico
	byte      iniBtn       , //pin de inicio de busqueda
	colorLDR* RGB          , //pin de la LDR del sensor de color
	float     diamRueda    , float anchoRobot) : //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, RGB, diamRueda, anchoRobot),
	iniBtn(iniBtn)
{
	setEstado(ESPERANDO);
	pinMode(iniBtn, INPUT_PULLUP);
}
//Fin: Constructores: Hormiga exploradora *************************************

void HormigaExploradora::enviarIR(float datos){
	for (int i = 0; i < 3; i++) {
		irsend.sendSony(*reinterpret_cast<unsigned long*>(&datos), 32); // Sony TV power code
		delay(100);
	}
	setEstado(ESPERANDO);
}

void HormigaExploradora::iniBusqueda(){ setEstado(BUSCANDO); }

//realiza el siguiente trabajo pendiente (basado en el estado)
void HormigaExploradora::trabajar(){
	switch (getEstado()){
		case ESPERANDO:
			if(!digitalRead(iniBtn)) setEstado(BUSCANDO);
			break;
		case BUSCANDO:
			desplazar();
			break;
		case ENCONTRADA:
			retornar();
			break;
		case COMUNICANDO:
			enviarIR((float)getVector()[1]);
			break;
	};
}
