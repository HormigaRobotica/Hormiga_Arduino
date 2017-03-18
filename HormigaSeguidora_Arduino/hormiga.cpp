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
#define ROTAC_FRAC 1/4 //fracción de vuelta por desplazamiento
#define ESPERANDO 0
#define BUSCANDO 1
#define ENCONTRADA 3

Hormiga::Hormiga(
	SincSteps* despl,
	byte trigPin, byte echoPin, float dist,
	byte pinTransistorCNY, byte pinLEDCNY,
	int color,
	float diamRueda, float anchoRobot) :

	despl(despl),
	trigPin(trigPin), echoPin(echoPin), dist(dist),
	pinTransistorCNY(pinTransistorCNY), pinLEDCNY(pinLEDCNY), color(color),
	perimRueda(diamRueda * PI),
	perimRobot(anchoRobot * PI)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	colorTol = 2;
	vectorDesp[0] = vectorDesp[1] = 0;
  randomSeed(analogRead(A4));
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

//lee los datos del CNY
int Hormiga::leerColor(){
	return analogRead(pinTransistorCNY);
}

//calibra el color a reconocer como azucar
void Hormiga::calColor(){
  color = leerColor();
}

//establece la tolerancia para el sensor de color (por defecto = +-2)
void Hormiga::toleranciaColor(int colorTol){
	this->colorTol = colorTol;
}

//escoge de forma aleatoria una dirección hacia la cual ir
float Hormiga::escogeDir(){
	float fracCirc = (random(2) ? 1 : -1) * random(5) / 8; //genera la fracción del circulo sobre el cual se va a rotar
	
	int rotac = despl->getRatio() * fracCirc; //genera la fracción del circulo sobre el cual se va a rotar

	despl->despInv(rotac);

	return fracCirc * perimRueda * 360 / perimRobot; //retorna el ángulo de rotación de la hormiga
}

//se desplaza una distancia definida a menos que exista un obstaculo
bool Hormiga::desplazar(){
	int colorL;
	float angulo = 0;
	while(Ultrasonico() < dist){
		angulo += escogeDir();
		colorL = leerColor();

		if(colorL < (color + colorTol) && (color - colorTol) < colorL){
			estado = ENCONTRADA;
			return true;
		}
	}

	addVectorsPolar(vectorDesp[0], vectorDesp[1], perimRueda * ROTAC_FRAC, deg2rad(angulo), vectorDesp, vectorDesp + 1);

	despl->desp(despl->getRatio() * ROTAC_FRAC);
	return false;
}

void Hormiga::setEstado(byte e){ estado = e; }

void Hormiga::retornar(){

}

void Hormiga::trabajar(){
  switch (estado){
    case ESPERANDO:
      break;
    case BUSCANDO:
      desplazar();
      break;
    case ENCONTRADA:
      retornar();
      break;
  };
}

//***************************************************************************************************

HormigaSeguidora::HormigaSeguidora(SincSteps* sinc, //puntero a la instancia para la sinsronización de los motores
	byte pinTransistorCNY, byte pinLEDCNY, //pines del transistor y el LED del sensor de color
	float distObstaculo, //distancia a la cual se evitan los obstaculos
	IRrecv* IRRead, //lector del sensor infrarrojo
	byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
	byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
	int color, //color a considerar como "azucar"
	float diamRueda, float anchoRobot) :  //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, pinTransistorCNY, pinLEDCNY, color, diamRueda, anchoRobot),
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
	float distObstaculo, //distancia a la cual se evitan los obstaculos
	byte UltrasTrigPin, //Pin "Trigger" del sensor ultrasónico
	byte UltrasEchoPin, //Pin "Echo" del sensor ultrasónico
	int color, //color a considerar como "azucar"
	float diamRueda, float anchoRobot) : //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, pinTransistorCNY, pinLEDCNY, color, diamRueda, anchoRobot)
{
	setEstado(ESPERANDO);
}

void HormigaExploradora::enviarIR(float datos){
	for (int i = 0; i < 3; i++) {
		irsend.sendSony(*reinterpret_cast<unsigned long*>(&datos), 32); // Sony TV power code
		delay(100);
	}
}

void HormigaExploradora::iniBusqueda(){ setEstado(BUSCANDO); }

