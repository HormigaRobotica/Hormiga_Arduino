/*
* Periodo 2017-1
* U.N.E.F.A. - N?cleo Caracas
* Carrera: Ingenier? Electr?ica
* Materia: Introducci? a la Rob?ica
*
* Hormiga (Seguidora)
*
* Este programa se encarga de controlar a una hormiga rob?ica
* que recibe la posici? de un objeto y se desplaza hacia el mismo una vez recibida esta informaci?
*/

#include "hormiga.h"

#define PPR 200       //pasos por revolucion para los motores
#define diamRueda 6   //diametro de la rueda [cm]
#define distRuedas 16 //distancia entre las ruedas [cm]
#define dist      15            //distancia a considerar para obstaculos
#define azucarRGB 230, 240, 245 //color del azucar

#define trigPin   12          //pin "Trigger" del sensor ultras?ico
#define echoPin   13          //pin "Echo" del sensor ultras?ico
#define IRreadpin  3          //pin para el receptor infrarrojo
#define LDR       A0          //pin de la LDR del sensor de color
#define RGB_LEDS    5,  6,  7 //pines de los lods del sensor de color
#define Motor1    PPR, 10, 11 //motor1: velocidad, pines
#define Motor2    PPR,  8,  9 //motor2: velocidad, pines


//Stepper MP(STEPS, 11,9,10,8);
SincSteps motores(Motor1, Motor2);
colorLDR color(RGB_LEDS, LDR);
HormigaSeguidora hormiga(&motores, dist, new IRrecv(IRreadpin), trigPin, echoPin, &color, azucarRGB, diamRueda, distRuedas);


float resultados;
void setup() {
  Serial.begin(9600);
  hormiga.enableIR();
  hormiga.setVector(30, 0.7854);
  hormiga.setUltAng(0.7854);
}

void loop() {
  hormiga.seguir();
  int *col=color.leerColor();
  Serial.print("r: ");Serial.print(col[0]);Serial.print("g: ");Serial.print(col[1]);Serial.print("b: ");Serial.println(col[2]);
  //Serial.println(hormiga.Ultrasonico());
  //motores.desp(15);

  //Serial.println(hormiga.getEstado());

  delay(500);
}
