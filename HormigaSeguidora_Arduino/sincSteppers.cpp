/*
* Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier?a Electr?nica
* Materia: Introducci?n a la Rob?tica
*
* sincSteppers.cpp
*
* Libreria para sincronizar el movimiento de 2 motores
*/

#include "sincSteppers.h"

#define EN_DISABLED 255

// Constructor Sobrecargado
SincSteps::SincSteps(const int pasosRev1,
	byte pin1Motor1,
	byte pin2Motor1,
	const int pasosRev2,
	byte pin1Motor2,
	byte pin2Motor2) :
	rpms(25)
{
	motor[0][0] = pin1Motor1; motor[0][1] = pin2Motor1; motor[0][4] = 2;
	motor[1][0] = pin1Motor2; motor[1][1] = pin2Motor2; motor[1][4] = 2;
	
	motor[0][5] = motor[1][5] = 0;
	motor[0][6] = motor[1][6] = EN_DISABLED;

	calcPasosDesp(pasosRev1, pasosRev2);

	for(int i=0; i<2; i++){
		for(int j=0; j<motor[i][4]; j++)
			pinMode(motor[i][j],OUTPUT);
	}
}

SincSteps::SincSteps(const int pasosRev1,
	byte pin1Motor1,
	byte pin2Motor1,
	byte pin3Motor1,
	byte pin4Motor1,
	const int pasosRev2,
	byte pin1Motor2,
	byte pin2Motor2,
	byte pin3Motor2,
	byte pin4Motor2) :
	rpms(25)
{
	motor[0][0] = pin1Motor1; motor[0][1] = pin2Motor1; motor[0][2] = pin3Motor1; motor[0][3] = pin4Motor1; motor[0][4] = 4;
	motor[1][0] = pin1Motor2; motor[1][1] = pin2Motor2; motor[1][2] = pin3Motor2; motor[1][3] = pin4Motor2; motor[1][4] = 4;

	motor[0][5] = motor[1][5] = 0;
	motor[0][6] = motor[1][6] = EN_DISABLED;

	calcPasosDesp(pasosRev1, pasosRev2);

	for(int i=0; i<2; i++){
		for(int j=0; j<motor[i][4]; j++)
			pinMode(motor[i][j],OUTPUT);
	}
}

SincSteps::SincSteps(const int pasosRev1,
	byte pin1Motor1,
	byte pin2Motor1,
	byte pin3Motor1,
	byte pin4Motor1,
	const int pasosRev2,
	byte pin1Motor2,
	byte pin2Motor2) :
	rpms(25)
{
	motor[0][0] = pin1Motor1; motor[0][1] = pin2Motor1; motor[0][2] = pin3Motor1; motor[0][3] = pin4Motor1; motor[0][4] = 4;
	motor[1][0] = pin1Motor2; motor[1][1] = pin2Motor2; motor[1][4] = 2;
	
	motor[0][5] = motor[1][5] = 0;
	motor[0][6] = motor[1][6] = EN_DISABLED;

	calcPasosDesp(pasosRev1, pasosRev2);
	
	for(int i=0; i<2; i++){
		for(int j=0; j<motor[i][4]; j++)
			pinMode(motor[i][j],OUTPUT);
	}
}

SincSteps::SincSteps(const int pasosRev1,
	byte pin1Motor1,
	byte pin2Motor1,
	const int pasosRev2,
	byte pin1Motor2,
	byte pin2Motor2,
	byte pin3Motor2,
	byte pin4Motor2) :
	rpms(25)
{
	motor[0][0] = pin1Motor1; motor[0][1] = pin2Motor1; motor[0][4] = 2;
	motor[1][0] = pin1Motor2; motor[1][1] = pin2Motor2; motor[1][2] = pin3Motor2; motor[1][3] = pin4Motor2; motor[1][4] = 4;
	
	
	motor[0][5] = motor[1][5] = 0;
	motor[0][6] = motor[1][6] = EN_DISABLED;

	calcPasosDesp(pasosRev1, pasosRev2);

	for(int i=0; i<2; i++){
		for(int j=0; j<motor[i][4]; j++)
			pinMode(motor[i][j],OUTPUT);
	}
}
// Fin Constructor Sobrecargado

//calcula la cantidad(minima) de pasos necesarios para un desplazamiento uniforme de ambos motores
void SincSteps::calcPasosDesp(const int pasosRev1, const int pasosRev2){
	ratio = gcd(pasosRev1, pasosRev2);
	pasosDesp[0] = pasosRev1 / ratio;
	pasosDesp[1] = pasosRev2 / ratio;
}

//rota cada uno de los motores hasta llegar al desplazamiento requerido
void SincSteps::desp(long nDesp){
	for (long i = 0; i < abs(nDesp); ++i){
		Step(0, ((nDesp < 0) ? -1 : 1) * pasosDesp[0]);
		Step(1, ((nDesp < 0) ? -1 : 1) * pasosDesp[1]);
	}
}

//rota cada uno de los motores en direcciones opuestas hasta llegar al desplazamiento requerido
void SincSteps::despInv(long nDesp){
	//Serial.println(nDesp);
	for (long i = 0; i < abs(nDesp); ++i){
		Step(0, ((nDesp < 0) ? -1 : 1) * pasosDesp[0]);
		Step(1, ((nDesp < 0) ? 1 : -1) * pasosDesp[1]);
	}
}

void SincSteps::Step(byte n, long steps){
	byte jm = 1, aux;

	if(motor[n][6] < 20) digitalWrite(motor[n][6], HIGH); //activa los motores si el pin enable esta habilitado

	if(steps < 0){
		steps *= -1;
		jm *= -1;
	}

	for(long i=0; i<steps; i++, motor[n][5] += jm){
		motor[n][5] = (motor[n][5]==4) ? 0 : (motor[n][5]==255) ? 3 : motor[n][5];
		aux = (motor[n][5] & 2) ? (motor[n][5] ^ 1) : motor[n][5];

		digitalWrite(motor[n][1], bitRead(aux, 1));
		digitalWrite(motor[n][0], bitRead(aux, 0));
		
		//Serial.print("motor "); Serial.print(n); Serial.print(": "); Serial.println(motor[n][5]);

		if (motor[n][4] == 4){
			digitalWrite(motor[n][3], !bitRead(aux, 1));
			digitalWrite(motor[n][2], !bitRead(aux, 0));
		}

		delay(60000 / (rpms * ratio));
	}

	if(motor[n][6] < 20) digitalWrite(motor[n][6], LOW); //desactiva los motores si el pin enable esta habilitado
}

//establece el pin de activación de los motores
void SincSteps::setEnPin (byte n, byte enPin){
	motor[n][6] = enPin;
	
	pinMode(enPin, OUTPUT);
	digitalWrite(enPin, LOW);
}

//rota cada uno de los motores (vueltas completas) en direcciones opuestas hasta llegar al desplazamiento requerido
void SincSteps::revInv(long nRev){ despInv(nRev * ratio); }

//retorna el numero de pasos para que el motor referenciado por "nMotor" de 1 desplazamiento
int SincSteps::getPasosDesp(byte nMotor){ return (nMotor == 0) ? pasosDesp[0] : pasosDesp[1]; }

//establece la velocidad de los motores
void SincSteps::setSpeed(unsigned long rpms){ this->rpms = rpms; }

//rota cada uno de los motores (vueltas completas) hasta llegar al desplazamiento requerido
void SincSteps::rev(long nRev){ desp(nRev * ratio); }

//retorna ratio de rotación
int SincSteps::getRatio(){ return ratio; }
