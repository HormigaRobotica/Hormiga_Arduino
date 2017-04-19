/*
*	Periodo 2017-1
*	U.N.E.F.A. - N?cleo Caracas
*	Carrera: Ingenier? Electr?ica
*	Materia: Introducci? a la Rob?ica
*
*	hormiga.h
*
*	Libreria para el funcionamiento estandar de las "hormigas" roboticas en este proyecto
*/

#ifndef HORMIGA_H
#define HORMIGA_H

#define ESPERANDO   0
#define BUSCANDO    1
#define ENCONTRADA  2
#define COMUNICANDO 3
#define SIGUIENDO   4

//DEBUG SNIPPETS ******************************************

#define PRINT_ULTRAS(u) { Serial.print("Ultrasonico: "); Serial.println(u); }

#define PRINT_ULTRAS_Y_COLOR(u, c) { PRINT_ULTRAS(us) PRINT_COLOR(c) }

//Fin: DEBUG SNIPPETS *************************************

#include <IRremote.h>
#include "sincSteppers.h"
#include "colorLDR.h"

class Hormiga {
	public:
		bool desplazar (); //se desplaza una distancia definida a menos que exista un obstaculo
		bool testColor (); //lee y compara el color actual con el color en memoria
		int  getPpg    (); //retorna el n?mero de pasos por giro de la hormiga

		void toleranciaColor(int colorTol); //establece la tolerancia para el sensor de color (por defecto = +-2)

		void setVector    (double m, double a); //asigna valores al vector de desplazamiento
		void setVectorMod (double m); //asigna el m?ulo del vector de desplazamiento
		void setVectorAng (double a); //asigna el ?gulo del vector de desplazamiento
		void setUltAng    (double a); //asigna el ?gulo de la ?ltima rotaci?
		void rotarFrac    (float  n); //rota la hormiga una fracci? de giro dada por "n"
		void setEstado    (byte   e); //establece el estado actual de trabajo
		void setDist      (float  d); //establece la distancia a la que se detectan obstaculos
		
		//establece el color a comparar
		void setColor(int* col);
		void setColor(int r, int g, int b);
		
		double* getVector     (); //retorna el vector desplazamiento
		float   Ultrasonico   (); //lee los datos del ultras?ico
		float   escogeDir     (); //escoge de forma aleatoria una direcci? hacia la cual ir
		float   getPerimRueda (); //retorna el perimetro de las ruedas
		float   getPerimRobot (); //retorna el perimetro de la hormiga
		float   getDist       (); //retorna la distancia a la cual se detectan obstaculos
		void    calHormiga    (); //calibra la hormiga
		void    retornar      (); //retorna hacia la posici? original
		void    seguir        (); //lleva la hormiga hacia el azucar
		byte    getEstado     (); //retorna el estado de trabajo actual
		int     getColorTol   (); //retorna el valor de tolerancia para las mediciones de color

		SincSteps* getDespl      (); //retorna el puntero al control de los motores

		virtual void trabajar  () =0; //realiza el siguiente trabajo pendiente (basado en el estado)

	protected:
		Hormiga(SincSteps* despl,
			byte      trigPin  ,
			byte      echoPin  ,
			float     dist     ,
			colorLDR* RGB      ,
			byte      colorR   , byte   colorG   , byte colorB,
			float     diamRueda, float anchoRobot);

		Hormiga(SincSteps* despl,
			byte      trigPin  ,
			byte      echoPin  ,
			colorLDR* RGB      ,
			float     diamRueda, float anchoRobot);

		int calCmdParser(String* cmd); //retorna el código del comando a revisar

	private:
		SincSteps* despl; //puntero a la instancia para la sinsronizaci? de los motores
		colorLDR*  RGB;   //puntero a la instancia del sensor de color

		byte trigPin; //Pin "Trigger" del sensor ultras?ico
		byte echoPin; //Pin "Echo" del sensor ultras?ico
		byte estado;  //estado actual de trabajo
		
		int color[3]; //color a considerar como "azucar"
		int colorTol; //tolerancia del color
		int ppg;      //pasos por giro de la hormiga

		float  dist;          //distancia a la cual se evitan los obstaculos
		float  factorCorr;    //factor de corrección del ultrasónico
		double vectorDesp[2]; //vector desplazamiento {modulo, ?gulo}
		double ultAngulo;     //?gulo de la ?ltima rotaci?

		const float perimRueda; //perimetro de las ruedas
		const float perimRobot; //perimetro del robot
};
#endif
