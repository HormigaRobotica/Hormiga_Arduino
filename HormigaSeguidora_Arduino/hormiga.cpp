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
#define TOL_COLOR 25 //tolerancia inicial en los valores de color
#define testDist(u, d) ((u) <= (d) && (u) >= 0)

//codigos para comandos de calibracion *****************************************
#define ULTRASOUNDCORR_INC 0
#define ULTRASOUNDCORR_DEC 1
#define ULTRASOUNDCORR_SET 2
#define ULTRASOUND_TEST 3
#define ULTRASOUND_SET 4

#define COLORTOL_INC 5
#define COLORTOL_DEC 6
#define COLORTOL_SET 7
#define COLOR_TEST 8
#define COLOR_SET 9

#define CMD_HELP 14
#define CMD_END 15

//CONFIGURACION PARA CALIBRACION
#define TEST_DIST_CONTA 4.0

#define CMD_SET_SIZE 4
#define CMD_SET_MAX 15

#define CMD_ERR -1
#define CMD_ERR_MSG "Error: el valor introducido no se reconoce como un comando valido"

/*
#define CMD_HELP_MSG "\"n\": número\n" \
	"\n" \
	"//Comandos para Ultrasonido\n" \
	"    //factor de corrección del ultrasónico\n" \
	"        ultras c ++  //incrementar\n" \
	"        ultras c --  //decrementar\n" \
	"        ultras c n   //establecer\n" \
	"        ultras p     //probar\n" \
	"    //distancia de detección del ultrasónico\n" \
	"        ultras s     //establecer\n" \
	"\n" \
	"//Comandos para Color\n" \
	"    //tolerancia de color\n" \
	"        color t ++  //incrementar\n" \
	"        color t --  //decrementar\n" \
	"        color t n   //establecer\n" \
	"        color p     //probar\n" \
	"    //color a detectar\n" \
	"        color s     //establecer\n" \
	"\n" \
	"//ayuda para calibración\n" \
	"[? || help || ayuda]" \
	"\n" \
	"//finalizar calibración\n" \
	"fin\n"
*/
//FIN: CONFIGURACION PARA CALIBRACION

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
	
	mem.conta = 1;
	mem.overfVal = mem.datos[0][1] = mem.datos[0][0] = 0;

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

	mem.conta = 1;
	mem.overfVal = mem.datos[0][1] = mem.datos[0][0] = 0;

	vectorDesp[0] = vectorDesp[1] = 0;
	randomSeed(analogRead(A4));
}
//Fin: Constructores: Hormiga *************************************

//lee y compara el color actual con el color en memoria
bool Hormiga::testColor(){
	int *colorL = RGB->leerColor();
	
	PRINT_COLOR(colorL);

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
						Serial.print("Correccion de ultrasonido: ");
						Serial.println(factorCorr);
						break;
					case ULTRASOUND_TEST:
						while(!(Serial.available() > 0)){
							PRINT_ULTRAS(Ultrasonico());
							delay(500);
						}
						break;
					case ULTRASOUND_SET:
						dist = Ultrasonico();
						PRINT_ULTRAS(dist);
						break;
					case COLORTOL_INC:
						toleranciaColor(colorTol + 1);

						Serial.print("tolerancia: ");
						Serial.println(colorTol);
						Serial.print((testColor()) ? "Color Encontrado!! " : "NO es el Color ");
						Serial.print("Ref: ");
						PRINT_COLOR(color);
						break;
					case COLORTOL_DEC:
						toleranciaColor(colorTol - 1);

						Serial.print("tolerancia: ");
						Serial.println(colorTol);
						Serial.print((testColor()) ? "Color Encontrado!! " : "NO es el Color ");
						Serial.print("Ref: ");
						PRINT_COLOR(color);
						break;
					case COLORTOL_SET:
						toleranciaColor(cmd.toInt());

						Serial.print("tolerancia: ");
						Serial.println(colorTol);
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

					#ifdef CMD_HELP_MSG
					case CMD_HELP:
						Serial.println(CMD_HELP_MSG);
						return;
					#endif

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
				retval = ULTRASOUNDCORR_SET;
		}
		else if(cmd->equals("s"))
			retval = ULTRASOUND_SET;
		else if(cmd->equals("p"))
			retval = ULTRASOUND_TEST;
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
				retval = COLORTOL_SET;
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

	#ifdef CMD_HELP_MSG
	else if(cmd->equals("?") || cmd->equals("help") || cmd->equals("ayuda"))
		retval = CMD_HELP;
	#endif

	return retval;
}

//revisa si el area actual ya existe en la memoria
bool Hormiga::existeEnMem(float x, float y){
	float len, d[2];

	for (byte i = 1; i < mem.conta; ++i){
		for (byte j = 0; j < 2; ++j){
			//distancias entre los vertices y el punto
			d[0] = (j ? y:x) - mem.datos[i-1][j];
			d[1] = (j ? y:x) - mem.datos[i][j];
			d[0] = abs(d[0]);
			d[1] = abs(d[1]);

			//longitudes horizontal y vertical del rectangulo
			len = mem.datos[i-1][j] - mem.datos[i][j];
			len = abs(len);
			
			if(len > d[0] && len > d[1])
				if(j==1) return true;
			else
				break;
		}
	}
	return false;
}

//dado el numero de pasos retorna el ángulo de rotación
double Hormiga::calcAngRotacion(int pasos){ return 2 * PI * perimRueda * pasos / (perimRobot * despl->getRatio()); }

//escoge de forma aleatoria una direcci? hacia la cual ir
float Hormiga::escogeDir(){
	double x, y;
	int pasos;

	for (byte i = 0; i < 16; ++i){
		pasos = (random(2) ? 1 : -1) * random(despl->getRatio() / 2); //genera la fracci? del circulo sobre el cual se va a rotar

		addVectorsPolar<double>(vectorDesp[0], vectorDesp[1], ROTAC_FRAC, calcAngRotacion(pasos), &x, &y);

		toCartesian(x, y, &x, &y);

		if(!existeEnMem(x, y)) break;
	}

	despl->despInv(pasos);

	return calcAngRotacion(pasos); //retorna el ?gulo de rotaci? de la hormiga
}

//se desplaza una distancia definida a menos que exista un obstaculo
bool Hormiga::desplazar(){
	int i;
	float us;

	ultAngulo = normalizeAngle(escogeDir());

	//se desplaza una distancia definida (o hasta encontrar un obstaculo)
	for (i = 0; i < despl->getRatio() * ROTAC_FRAC; ++i){
		us = Ultrasonico();
		if(testDist(us, dist)){
			for (int j = 0; j < TEST_DIST_CONTA; ++j){
				if (testColor()){
					addVectorsPolar<double>(vectorDesp[0], vectorDesp[1], (i * perimRueda / despl->getRatio()), ultAngulo, vectorDesp, vectorDesp + 1);
					vectorDesp[1] = normalizeAngle(vectorDesp[1]);
					return true;
				}

				float d = dist / TEST_DIST_CONTA;
				int val = (despl->getRatio() * d) / perimRueda;

				despl->desp(val);
				
				if(j < (TEST_DIST_CONTA - 1))
					vectorDesp[0] += d;
				else{
					despl->desp(-(val * j));
					vectorDesp[0] -= d *  j;

					rotarFrac(0.5);
					ultAngulo += PI;
				}
			}
			break;
		}
		despl->desp(1);
	}

	addVectorsPolar<double>(vectorDesp[0], vectorDesp[1], (i * perimRueda / despl->getRatio()), ultAngulo, vectorDesp, vectorDesp + 1);
	vectorDesp[1] = normalizeAngle(vectorDesp[1]);

	if(mem.conta < HORMIGA_MEM){
		mem.datos[mem.conta][0] = vectorDesp[0];
		mem.datos[mem.conta][1] = vectorDesp[1];
		mem.conta++;
	}
	else{
		mem.datos[mem.overfVal][0] = vectorDesp[0];
		mem.datos[mem.overfVal][1] = vectorDesp[1];
		mem.overfVal = (mem.overfVal < (HORMIGA_MEM - 1)) ? mem.overfVal + 1 : 0;
	}

	return false;
}

//retorna hacia la posici? original
void Hormiga::retornar(){
	rotarFrac  ( 0.5 + (ultAngulo / (2 * PI)) );
	int dist = despl->getRatio() * vectorDesp[0] / perimRueda;

	for (int i = 0; i < dist || testDist(Ultrasonico(), dist); ++i)
		despl->desp(1);

	setEstado  (COMUNICANDO);
}

//lleva la hormiga hacia el azucar
void Hormiga::seguir(){
	float us;

	rotarFrac( (float)vectorDesp[1] / (2 * PI) );

	/*Nota: Agregar an?isis de color*/
	us = Ultrasonico();
	while(!testDist(us, dist)){
		despl->desp(1);
		us = Ultrasonico();
	}
	
	setEstado(ENCONTRADA);
}

//rota la hormiga una fracci? de giro dada por "n" Ej: n=0.5 (media vuelta)
void Hormiga::rotarFrac(float n){ despl->despInv( n * despl->getRatio() * perimRobot / perimRueda ); }

//Funciones de asignaci?****************************************************
void Hormiga::setEstado      ( byte   e ){ estado        = e; } //establece el estado actual de trabajo
void Hormiga::setVectorMod   ( double m ){ vectorDesp[0] = m; } //asigna el m?ulo del vector de desplazamiento
void Hormiga::setVectorAng   ( double a ){ vectorDesp[1] = a; } //asigna el ?gulo del vector de desplazamiento
void Hormiga::setUltAng      ( double a ){ ultAngulo     = a; } //asigna el ?gulo de la ?ltima rotaci?
void Hormiga::setDist        ( float  d ){ dist          = d; } //establece la distancia a la que se detectan obstaculos
void Hormiga::setUltrasCorr  ( float  c ){ factorCorr    = c; } //establece el factor de corrección del ultrasonido
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
float      Hormiga::getPerimRueda(){ return perimRueda; } //retorna el perimetro de las ruedas
float      Hormiga::getPerimRobot(){ return perimRobot; } //retorna el perimetro de la hormiga
float      Hormiga::getDist      (){ return dist;       } //retorna la distancia a la cual se detectan obstaculos
double*    Hormiga::getVector    (){ return vectorDesp; } //retorna el vector desplazamiento
SincSteps* Hormiga::getDespl     (){ return despl;      } //retorna el puntero al control de los motores
//Fin: Funciones para valores en variables***********************************
