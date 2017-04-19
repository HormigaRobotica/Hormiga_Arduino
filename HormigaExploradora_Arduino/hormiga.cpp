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
#define TOL_COLOR 10 //tolerancia inicial en los valores de color
#define testDist(u, d) ((u) <= (d) && (u) >= 0)

//codigos para comandos de calibracion *****************************************
#define ULTRASOUNDCORR_INC 0
#define ULTRASOUNDCORR_DEC 1
#define ULTRASOUNDCORR_SET 2
#define ULTRASOUND_SET 3
#define COLORTOL_INC 4
#define COLORTOL_DEC 5
#define COLORTOL_SET 6
#define COLOR_TEST 7
#define COLOR_SET 8
#define CMD_END 15

#define CMD_SET_SIZE 4
#define CMD_SET_MAX 15

#define CMD_ERR -1
#define CMD_ERR_MSG "Error: el valor introducido no se reconoce como un comando valido"
//Fin: codigos para comandos de calibracion ************************************

//Constructores: Hormiga ******************************************

Hormiga::Hormiga(
	SincSteps* despl    ,
	byte       trigPin  , byte echoPin, float dist,
	colorLDR*  RGB      ,
	byte       colorR   , byte  colorG    , byte colorB,
	float      diamRueda, float anchoRobot) :

	despl     (despl  ),
	trigPin   (trigPin), echoPin(echoPin), dist(dist),
	RGB       (RGB),
	colorTol  (TOL_COLOR),
	perimRueda((float)diamRueda * PI ),
	perimRobot((float)anchoRobot * PI),
	factorCorr(1)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	setColor(colorR, colorG, colorB);

	ppg = perimRobot * despl->getRatio() / perimRueda;/*112*/
	

	vectorDesp[0] = vectorDesp[1] = 0;
	randomSeed(analogRead(A4));
}

Hormiga::Hormiga(
	SincSteps* despl    ,
	byte       trigPin  , byte echoPin,
	colorLDR*  RGB      ,
	float      diamRueda, float anchoRobot) :

	despl     (despl  ),
	trigPin   (trigPin), echoPin(echoPin),
	RGB       (RGB),
	colorTol  (TOL_COLOR),
	perimRueda((float)diamRueda * PI ),
	perimRobot((float)anchoRobot * PI),
	factorCorr(1)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	ppg = perimRobot * despl->getRatio() / perimRueda;/*112*/

	vectorDesp[0] = vectorDesp[1] = 0;
	randomSeed(analogRead(A4));
}
//Fin: Constructores: Hormiga *************************************

//lee y compara el color actual con el color en memoria
bool Hormiga::testColor(){
	int *colorL = RGB->leerColor();
	
	//PRINT_COLOR(colorL);

	if(RGB->compColor(colorL, color, colorTol)){
		estado = ENCONTRADA;
		return true;
	}
	return false;
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
	return ((((pulseIn(echoPin, HIGH) / 2) * 0.03438) -1) * factorCorr);
}

//calibra la hormiga
void Hormiga::calHormiga(){
	Serial.println("Bienvenido a la interfaz de calibracion");
	for (;;){
		if (Serial.available() > 0){
			int cmdCode;
			String cmd = Serial.readString();

			cmd.trim();
			cmdCode = calCmdParser(&cmd);

			if(cmdCode == CMD_ERR){
				Serial.println(CMD_ERR_MSG);
			}
			else{
				switch(cmdCode & CMD_SET_MAX){
					case ULTRASOUNDCORR_INC: 
						factorCorr += 0.1;
						PRINT_ULTRAS(Ultrasonico());
						break;
					case ULTRASOUNDCORR_DEC:
						factorCorr -= 0.1;
						PRINT_ULTRAS(Ultrasonico());
						break;
					case ULTRASOUNDCORR_SET:
						factorCorr = cmd.toFloat();
						break;
					case ULTRASOUND_SET:
						dist = Ultrasonico();
						PRINT_ULTRAS(dist);
						break;
					case COLORTOL_INC:
						toleranciaColor(getColorTol() + 1);

						Serial.print("tolerancia: ");
						Serial.println(getColorTol());
						Serial.print((testColor()) ? "Color Encontrado!! " : "NO es el Color ");
						Serial.print("Ref: ");
						PRINT_COLOR(color);
						break;
					case COLORTOL_DEC:
						toleranciaColor(getColorTol() - 1);

						Serial.print("tolerancia: ");
						Serial.println(getColorTol());
						Serial.print((testColor()) ? "Color Encontrado!! " : "NO es el Color ");
						Serial.print("Ref: ");
						PRINT_COLOR(color);
						break;
					case COLORTOL_SET:
						toleranciaColor(cmdCode >> CMD_SET_SIZE);

						Serial.print("tolerancia: ");
						Serial.println(getColorTol());
						Serial.print((testColor()) ? "Color Encontrado!! " : "NO es el Color ");
						Serial.print("Ref: ");
						PRINT_COLOR(color);
						break;
					case COLOR_TEST:
						while(!(Serial.available() > 0)){
							Serial.print((testColor()) ? "Color Encontrado!! " : "NO es el Color ");
							Serial.print("Ref: ");
							PRINT_COLOR(color);
						}
						break;
					case COLOR_SET:
						setColor(RGB->leerColor());
						PRINT_COLOR(color);
						break;
					case CMD_END:
						Serial.println("Calibracion finalizada");
						return;
				};
			}
		}

		delay(500);
	}
}

//retorna el código del comando a revisar
int Hormiga::calCmdParser(String* cmd){
	int retval = CMD_ERR;

	cmd->toLowerCase();
	
	Serial.println(*cmd);
	
	if(cmd->startsWith("ultras ")){
		cmd->remove(0, 6);
		cmd->trim();
		
		//Serial.println(*cmd);

		if(cmd->startsWith("c ")){
			cmd->remove(0, 1);
			cmd->trim();

			if(cmd->equals("++"))
				retval = ULTRASOUNDCORR_INC;
			else if(cmd->equals("--"))
				retval = ULTRASOUNDCORR_DEC;
			else
				retval = ULTRASOUNDCORR_SET + (cmd->toInt() << CMD_SET_SIZE);
		}
		else if(cmd->equals("s"))
			retval = ULTRASOUND_SET;
	}
	else if(cmd->startsWith("color ")){
		cmd->remove(0, 5);
		cmd->trim();
		
		//Serial.println(*cmd);

		if(cmd->startsWith("t ")){
			cmd->remove(0, 1);
			cmd->trim();

			if(cmd->equals("++"))
				retval = COLORTOL_INC;
			else if(cmd->equals("--"))
				retval = COLORTOL_DEC;
			else{
				retval = COLORTOL_SET + (cmd->toInt() << CMD_SET_SIZE);
				//Serial.println(retval, HEX);
			}
		}
		else if(cmd->equals("p"))
			retval = COLOR_TEST;
		else if(cmd->equals("s"))
			retval = COLOR_SET;
	}
	else if(cmd->equals("fin"))
		retval = CMD_END;

	return retval;
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
	while(testDist(us, dist)){
		if (testColor()) return true;

		rotarFrac(1.0/8.0);
		ultAngulo = normalizeAngle(ultAngulo + PI/4);
		us = Ultrasonico();
	}

	//se desplaza una distancia definida (o hasta encontrar un obstaculo)
	for (i = 0; i < despl->getRatio() * ROTAC_FRAC; ++i){
		us = Ultrasonico();
		if(testDist(us, dist)){
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
	while(!testDist(us, dist)){
		getDespl()->desp(1);
		us = Ultrasonico();
	}
	
	setEstado(ENCONTRADA);
}

//rota la hormiga una fracci? de giro dada por "n" Ej: n=0.5 (media vuelta)
void Hormiga::rotarFrac(float n){ despl->despInv( (float)ppg * (float)n ); }

//Funciones de asignaci?****************************************************
void Hormiga::setEstado   ( byte   e ){ estado        = e; } //establece el estado actual de trabajo
void Hormiga::setVectorMod( double m ){ vectorDesp[0] = m; } //asigna el m?ulo del vector de desplazamiento
void Hormiga::setVectorAng( double a ){ vectorDesp[1] = a; } //asigna el ?gulo del vector de desplazamiento
void Hormiga::setUltAng   ( double a ){ ultAngulo     = a; } //asigna el ?gulo de la ?ltima rotaci?
void Hormiga::setDist     ( float  d ){ dist          = d; } //establece la distancia a la que se detectan obstaculos
void Hormiga::toleranciaColor( int colorTol ){ this->colorTol = colorTol; } //establece la tolerancia para el sensor de color (por defecto = +-2)

//establece el color a comparar
void Hormiga::setColor( int* col ){ for (int i = 0; i < 3; ++i) color[i] = col[i]; }
void Hormiga::setColor(int r, int g, int b){
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

//asigna valores al vector de desplazamiento
void Hormiga::setVector(double m, double a){
	vectorDesp[0] = m;
	vectorDesp[1] = a;
}
//Fin: Funciones de asignaci?***********************************************

//Funciones para valores en variables****************************************
byte       Hormiga::getEstado    (){ return estado;     } //retorna el estado de trabajo actual
int        Hormiga::getColorTol  (){ return colorTol;    } //retorna el valor de tolerancia para las mediciones de color
int        Hormiga::getPpg       (){ return ppg;        } //retorna el n?mero de pasos por giro de la hormiga
float      Hormiga::getPerimRueda(){ return perimRueda; } //retorna el perimetro de las ruedas
float      Hormiga::getPerimRobot(){ return perimRobot; } //retorna el perimetro de la hormiga
float      Hormiga::getDist      (){ return dist;       } //retorna la distancia a la cual se detectan obstaculos
double*    Hormiga::getVector    (){ return vectorDesp; } //retorna el vector desplazamiento
SincSteps* Hormiga::getDespl     (){ return despl;      } //retorna el puntero al control de los motores
//Fin: Funciones para valores en variables***********************************
