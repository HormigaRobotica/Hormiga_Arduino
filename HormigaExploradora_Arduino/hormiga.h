/*
*	Periodo 2017-1
*	U.N.E.F.A. - N�cleo Caracas
*	Carrera: Ingenier�a Electr�nica
*	Materia: Introducci�n a la Rob�tica
*
*	hormiga.h
*
*	Libreria para el funcionamiento estandar de las "hormigas" roboticas en este proyecto
*/

#ifndef HORMIGA_H
#define HORMIGA_H

#include "sincSteppers.h"

class Hormiga {
  public:
    Hormiga(SincSteps* despl, int colorAzucar, int distObstaculo);
    void iniBusqueda();
  private:
    SincSteps* despl;
};

#endif
