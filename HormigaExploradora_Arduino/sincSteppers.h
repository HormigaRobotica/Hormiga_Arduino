/*
* Periodo 2017-1
* U.N.E.F.A. - N�cleo Caracas
* Carrera: Ingenier�a Electr�nica
* Materia: Introducci�n a la Rob�tica
*
* sincSteppers.h
*
* Libreria para sincronizar el movimiento de 2 motores
*/

#ifndef SINCSTEPPERS_H
#define SINCSTEPPERS_H

#include <Arduino.h>
#include <Stepper.h>
#include "extendMath.h"

class SincSteps{
  public:
    // Constructor Sobrecargado
    SincSteps(const int pasosRev1,
      byte pin1Motor1,
      byte pin2Motor1,
      const int pasosRev2,
      byte pin1Motor2,
      byte pin2Motor2);

    SincSteps(const int pasosRev1,
      byte pin1Motor1,
      byte pin2Motor1,
      byte pin3Motor1,
      byte pin4Motor1,
      const int pasosRev2,
      byte pin1Motor2,
      byte pin2Motor2,
      byte pin3Motor2,
      byte pin4Motor2);

    SincSteps(const int pasosRev1,
      byte pin1Motor1,
      byte pin2Motor1,
      byte pin3Motor1,
      byte pin4Motor1,
      const int pasosRev2,
      byte pin1Motor2,
      byte pin2Motor2);

    SincSteps(const int pasosRev1,
      byte pin1Motor1,
      byte pin2Motor1,
      const int pasosRev2,
      byte pin1Motor2,
      byte pin2Motor2,
      byte pin3Motor2,
      byte pin4Motor2);
    // Fin Constructor Sobrecargado

    void calcPasosDesp(const int pasosRev1, const int pasosRev2); //calcula el numero de pasos requerido para un desplazamiento uniforme
    void desp(long nDesp);             //rota cada uno de los motores hasta llegar al desplazamiento requerido
    void rev(long nRev);               //rota los motores vueltas completas
    void despInv(long nDesp);          //rota cada uno de los motores en direcciones opuestas hasta llegar al desplazamiento requerido
    void revInv(long nRev);            //rota los motores en direcciones opuestas vueltas completas
    int  getPasosDesp(byte nMotor);    //retorna el numero de pasos para que el motor referenciado por "nMotor" de 1 desplazamiento
    void setSpeed(unsigned long rpms); //establece la velocidad de los motores
    int  getRatio();                   //retorna ratio de rotación
    
  private:
    Stepper *motor[2];
    int pasosDesp[2];
    int ratio;
};

#endif
