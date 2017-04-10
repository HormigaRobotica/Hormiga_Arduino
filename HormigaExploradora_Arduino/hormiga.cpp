/*
*  Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier?a Electr?nica
* Materia: Introducci?n a la Rob?tica
*
* hormiga.cpp
*
* Libreria para el funcionamiento estandar de las "hormigas" roboticas en este proyecto
*/

#include "hormiga.h"
#define ROTAC_FRAC 1/2 //fracci? de vuelta por desplazamiento

Hormiga::Hormiga(
	SincSteps* despl     ,
	byte       trigPin   , byte echoPin    , float dist,
	colorLDR*  RGB       ,
	byte       colorR    , byte  colorG    , byte colorB,
	float      diamRueda , float anchoRobot) :

	despl     (despl  ),
	trigPin   (trigPin), echoPin(echoPin), dist(dist),
	RGB       (RGB),
	colorTol  (2  ),
	perimRueda((float)diamRueda * PI ),
	perimRobot((float)anchoRobot * PI)
{

	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;

	ppg = perimRobot * despl->getRatio() / perimRueda;/*112*/
	

	vectorDesp[0] = vectorDesp[1] = 0;
	randomSeed(analogRead(A4));

}

//lee y compara el color actual con el color en memoria
bool Hormiga::testColor(){
	int *colorL = RGB->leerColor();
		
	///*
	Serial.print("r: "); Serial.print(colorL[0]);
	Serial.print("    g: "); Serial.print(colorL[1]);
	Serial.print("    b: "); Serial.println(colorL[2]);
	//*/

	if(RGB->compColor(colorL, color, colorTol)){
		estado = ENCONTRADA;
		return true;
	}
}

//mide la distancia entre el ultrasonico y un obst?ulo
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

//escoge de forma aleatoria una direcci? hacia la cual ir
float Hormiga::escogeDir(){
	float fracCirc = (random(2) ? 1 : -1) * random(3) / 8.0; //genera la fracci? del circulo sobre el cual se va a rotar

	rotarFrac((float)fracCirc);

	return 2 * PI * fracCirc; //retorna el ?gulo de rotaci? de la hormiga
}

//se desplaza una distancia definida a menos que exista un obstaculo
bool Hormiga::desplazar(){
	int i;
	float us;

	ultAngulo = normalizeAngle(escogeDir());
	
	//rota una distancia definida siempre que encuentre un obst?ulo
	us = Ultrasonico();
	while(us <= dist && us >= 0){
		if (testColor()) return true;

		rotarFrac(1.0/8.0);
		ultAngulo = normalizeAngle(ultAngulo + PI/4);
		us = Ultrasonico();
	}

	//se desplaza una distancia definida (o hasta encontrar un obstaculo)
	for (i = 0; i < despl->getRatio() * ROTAC_FRAC; ++i){
		us = Ultrasonico();
		if(us <= dist && us >= 0){
			if (testColor()){
				addVectorsPolar(vectorDesp[0], vectorDesp[1], perimRueda * (i/despl->getRatio()), ultAngulo, vectorDesp, vectorDesp + 1);
				return true;
			}

			break;
		}
		despl->desp(1);
	}

	addVectorsPolar(vectorDesp[0], vectorDesp[1], perimRueda * (i/despl->getRatio()), ultAngulo, vectorDesp, vectorDesp + 1);
	return false;
}

//rota la hormiga una fracci? de giro dada por "n" Ej: n=0.5 (media vuelta)
void Hormiga::rotarFrac(float n){ despl->despInv( (float)ppg * (float)n ); }

//retorna hacia la posici? original
void Hormiga::retornar(){
	rotarFrac  ( -ultAngulo / (2 * PI) );
	despl->desp( getDespl()->getRatio() * vectorDesp[0] / perimRueda );
	setEstado  (COMUNICANDO);
}

//lleva la hormiga hacia el azucar
void Hormiga::seguir(){
	float us;

	rotarFrac  ( (float)getVector()[1] / (2 * PI) );

	/*Nota: Agregar an?isis de color*/
	us = Ultrasonico();
	while(us > getDist() || us < 0){
		getDespl()->desp(1);
	}
	setEstado(ENCONTRADA);
}

//Funciones de asignaci?****************************************************
void Hormiga::setEstado      (  byte e      ){ estado         = e;           } //establece el estado actual de trabajo
void Hormiga::setVectorMod   (  double m    ){ vectorDesp[0]  = m;           } //asigna el m?ulo del vector de desplazamiento
void Hormiga::setVectorAng   (  double a    ){ vectorDesp[1]  = a;           } //asigna el ?gulo del vector de desplazamiento
void Hormiga::setUltAng      (  double a    ){ ultAngulo      = a;           } //asigna el ?gulo de la ?ltima rotaci?
void Hormiga::toleranciaColor( int colorTol ){ this->colorTol = colorTol;    } //establece la tolerancia para el sensor de color (por defecto = +-2)

//asigna valores al vector de desplazamiento
void Hormiga::setVector(double m, double a){
	vectorDesp[0] = m;
	vectorDesp[1] = a;
}
//Fin: Funciones de asignaci?***********************************************

//Funciones para valores en variables****************************************
byte       Hormiga::getEstado    (){ return estado;     } //retorna el estado de trabajo actual
int        Hormiga::getPpg       (){ return ppg;        } //retorna el n?mero de pasos por giro de la hormiga
float      Hormiga::getPerimRueda(){ return perimRueda; } //retorna el perimetro de las ruedas
float      Hormiga::getPerimRobot(){ return perimRobot; } //retorna el perimetro de la hormiga
float      Hormiga::getDist      (){ return dist;       } //retorna la distancia a la cual se detectan obstaculos
double*    Hormiga::getVector    (){ return vectorDesp; } //retorna el vector desplazamiento
SincSteps* Hormiga::getDespl     (){ return despl;      } //retorna el puntero al control de los motores
//Fin: Funciones para valores en variables***********************************

//***************************************************************************************************

HormigaSeguidora::HormigaSeguidora(SincSteps* sinc,         //puntero a la instancia para la sinsronizaci? de los motores
	float     distObstaculo,                                //distancia a la cual se evitan los obstaculos
	IRrecv*   IRRead       ,                                //lector del sensor infrarrojo
	byte      UltrasTrigPin,                                //Pin "Trigger" del sensor ultras?ico
	byte      UltrasEchoPin,                                //Pin "Echo" del sensor ultras?ico
	colorLDR* RGB          ,
	byte      colorR       , byte  colorG    , byte colorB, //color a considerar como "azucar"
	float     diamRueda    , float anchoRobot) :            //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, RGB, colorR, colorG, colorB, diamRueda, anchoRobot),
	IRRead (IRRead),
	IRRes  (new decode_results)
{
}

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
			break;
	};
}

//***************************************************************************************************

HormigaExploradora::HormigaExploradora(SincSteps* sinc,     //puntero a la instancia para la sinsronizaci? de los motores
	float     distObstaculo,                                //distancia a la cual se evitan los obstaculos
	byte      UltrasTrigPin,                                //Pin "Trigger" del sensor ultras?ico
	byte      UltrasEchoPin,                                //Pin "Echo" del sensor ultras?ico
	colorLDR* RGB          ,
	byte      colorR       , byte  colorG    , byte colorB, //color a considerar como "azucar"
	float     diamRueda    , float anchoRobot) :            //diametro de la rueda y ancho del robot

	Hormiga(sinc, UltrasTrigPin, UltrasEchoPin, distObstaculo, RGB, colorR, colorG, colorB, diamRueda, anchoRobot)
{
	setEstado(ESPERANDO);
}

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
