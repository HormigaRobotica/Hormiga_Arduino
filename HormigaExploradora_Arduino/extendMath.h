/*
* ANDRBR 2017
*
* extendMath.h
*
* Library designed to add some math functionality
*/

#ifndef EXTEND_MATH_H
#define EXTEND_MATH_H

#include <math.h>

#ifndef PI
#define PI 3.1415926535897932
#endif

int gcd(int a, int b); //calculates the gratest common divisor between 2 numbers[gcd]

//conversions between radians and degrees
double deg2rad(double a);
double rad2deg(double a);

//Vector arithmetics*****************************************************

//adds vectors a + b and stores the result in s
void addVectors(double ax, double ay, double bx, double by, double *sx, double *sy); //being: a = {ax, ay} ; b = {bx, by}; s = {sx, sy}
void addVectorsPolar(double ar, double aphi, double br, double bphi, double *sr, double *sphi); //being: a = {ar, aphi} ; b = {br, bphi}; s = {sr, sphi}

//END Vector arithmetics*************************************************

#endif
