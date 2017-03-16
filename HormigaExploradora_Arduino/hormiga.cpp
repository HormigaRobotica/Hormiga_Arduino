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
	byte pinTransistorCNY, byte pinLEDCNY,
	int color) :

	despl(despl),
	trigPin(trigPin), echoPin(echoPin), dist(dist),
	pinTransistorCNY(pinTransistorCNY), pinLEDCNY(pinLEDCNY), color(color)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
  colorTol = 2;
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

//calibra el color a reconocer como azucar
void Hormiga::calColor(){
  color = analogRead(pinTransistorCNY);
}

//establece la tolerancia para el sensor de color (por defecto = +-2)
void Hormiga::toleranciaColor(int colorTol){
	this->colorTol = colorTol;
}

//***************************************************************************************************

HormigaSeguidora::HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
	byte pinTransistorCNY, byte pinLEDCNY, //pines del transistor y el LED del sensor de color
	int distObstaculo, //distancia a la cual se evitan los obstaculos
	IRrecv* IRRead, //lector del sensor infrarrojo
	byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
	byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
	int color) : //color a considerar como "azucar"

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, pinTransistorCNY, pinLEDCNY, color),
	IRRead(IRRead),
	IRRes(new decode_results)
{
}

bool HormigaSeguidora::chkIR(){
	if (IRRead->decode(IRRes)){
		datosIR = *reinterpret_cast<float*>(&IRRes->value);
		IRRead->resume();
		return true;
	}
	return false;
}

float HormigaSeguidora::getDatIR(){ return datosIR; }
void HormigaSeguidora::enableIR(){ IRRead->enableIRIn(); }

//***************************************************************************************************

HormigaExploradora::HormigaExploradora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
	byte pinTransistorCNY, byte pinLEDCNY, //pines del transistor y el LED del sensor de color
	int distObstaculo, //distancia a la cual se evitan los obstaculos
	byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
	byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
	int color) : //color a considerar como "azucar"

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, pinTransistorCNY, pinLEDCNY, color)
{
}

void HormigaExploradora::enviarIR(float datos){
	for (int i = 0; i < 3; i++) {
		irsend.sendSony(*reinterpret_cast<unsigned long*>(&datos), 32); // Sony TV power code
		delay(100);
	}
}
