/*
*  Periodo 2017-1
* U.N.E.F.A. - N�cleo Caracas
* Carrera: Ingenier�a Electr�nica
* Materia: Introducci�n a la Rob�tica
*
* hormiga.cpp
*
* Libreria para el funcionamiento estandar de las "hormigas" roboticas en este proyecto
*/

#include "hormiga.h"

Hormiga::Hormiga(
	SincSteps* despl,
	byte trigPin, byte echoPin, int dist,
	byte pinLDR, byte rgbLEDPin1, byte rgbLEDPin2, byte rgbLEDPin3,
	int r, int g, int b) :

	despl(despl),
	trigPin(trigPin), echoPin(echoPin), dist(dist),
	pinLDR(pinLDR)
{
  rgbLEDsPins[0] = rgbLEDPin1;
  rgbLEDsPins[1] = rgbLEDPin2;
  rgbLEDsPins[2] = rgbLEDPin3;
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

//mide la distancia entre el ultrasonico y un obstáculo
float Hormiga::Ultrasonico(){
	//envia la onda de sonido
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	//retorna la distancia a la cual se encuentra el sensor ultrasonico
	return (((pulseIn(echoPin, HIGH) / 2) * 0.03438) -1);
}

HormigaSeguidora::HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
	byte LDRPin, //pin para LDR del sensor de color
	byte rgbLEDPin1, byte rgbLEDPin2, byte rgbLEDPin3, //pines para los LEDs del sensor de color
	int distObstaculo, //distancia a la cual se evitan los obstaculos
	IRrecv* IRRead, //lector del sensor infrarrojo
	byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
	byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
	int r, int g, int b) : /*color a considerar como "azucar")*/

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, LDRPin, rgbLEDPin1, rgbLEDPin2, rgbLEDPin3, r, g, b),
	IRRead(IRRead),
	IRRes(new decode_results)
{
}

bool HormigaSeguidora::chkIR(){
	if (IRRead->decode(IRRes)) {
		datosIR = *reinterpret_cast<float*>(&IRRes->value);
		IRRead->resume();
		return true;
	}
	return false;
}

float HormigaSeguidora::getDatIR(){ return datosIR; }
void HormigaSeguidora::enableIR(){ IRRead->enableIRIn(); }
