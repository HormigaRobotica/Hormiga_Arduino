/*
* Periodo 2017-1
* U.N.E.F.A. - N�cleo Caracas
* Carrera: Ingenier�a Electr�nica
* Materia: Introducci�n a la Rob�tica
*
* sincSteppers.cpp
*
* Libreria para sincronizar el movimiento de 2 motores
*/

#include "sincSteppers.h"

// Constructor Sobrecargado
SincSteps::SincSteps(const int pasosRev1,
  byte pin1Motor1,
  byte pin2Motor1,
  const int pasosRev2,
  byte pin1Motor2,
  byte pin2Motor2)
{
  motor[0] = new Stepper(pasosRev1, pin1Motor1, pin2Motor1);
  motor[1] = new Stepper(pasosRev2, pin1Motor2, pin2Motor2);
  calcPasosDesp(pasosRev1, pasosRev2);
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
  byte pin4Motor2)
{
  motor[0] = new Stepper(pasosRev1, pin1Motor1, pin2Motor1, pin3Motor1, pin4Motor1);
  motor[1] = new Stepper(pasosRev2, pin1Motor2, pin2Motor2, pin3Motor2, pin4Motor2);
  calcPasosDesp(pasosRev1, pasosRev2);
}

SincSteps::SincSteps(const int pasosRev1,
  byte pin1Motor1,
  byte pin2Motor1,
  byte pin3Motor1,
  byte pin4Motor1,
  const int pasosRev2,
  byte pin1Motor2,
  byte pin2Motor2)
{
  motor[0] = new Stepper(pasosRev1, pin1Motor1, pin2Motor1, pin3Motor1, pin4Motor1);
  motor[1] = new Stepper(pasosRev2, pin1Motor2, pin2Motor2);
  calcPasosDesp(pasosRev1, pasosRev2);
}

SincSteps::SincSteps(const int pasosRev1,
  byte pin1Motor1,
  byte pin2Motor1,
  const int pasosRev2,
  byte pin1Motor2,
  byte pin2Motor2,
  byte pin3Motor2,
  byte pin4Motor2)
{
  motor[0] = new Stepper(pasosRev1, pin1Motor1, pin2Motor1);
  motor[1] = new Stepper(pasosRev2, pin1Motor2, pin2Motor2, pin3Motor2, pin4Motor2);
  calcPasosDesp(pasosRev1, pasosRev2);
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
  for (long i = 0; i < nDesp; ++i){
    motor[0]->step(pasosDesp[0]);
    motor[1]->step(pasosDesp[1]);
  }
}

//rota cada uno de los motores (vueltas completas) hasta llegar al desplazamiento requerido
void SincSteps::rev(long nRev){
  desp(nRev * ratio);
}

//rota cada uno de los motores en direcciones opuestas hasta llegar al desplazamiento requerido
void SincSteps::despInv(long nDesp){
  for (long i = 0; i < nDesp; ++i){
    motor[0]->step(pasosDesp[0]);
    motor[1]->step(-pasosDesp[1]);
  }
}

//rota cada uno de los motores (vueltas completas) en direcciones opuestas hasta llegar al desplazamiento requerido
void SincSteps::revInv(long nRev){
  despInv(nRev * ratio);
}

//retorna el numero de pasos para que el motor referenciado por "nMotor" de 1 desplazamiento
int SincSteps::getPasosDesp(byte nMotor){
  return (nMotor == 0) ? pasosDesp[0] : pasosDesp[1];
}

//establece la velocidad de los motores
void SincSteps::setSpeed(unsigned long rpms){
  motor[0]->setSpeed(rpms);
  motor[1]->setSpeed(rpms);
}

int SincSteps::getRatio(){
  return ratio;
}