/*
* ANDRBR 2017
*
* extendMath.h
*
* Library designed to add some math functionality
*/

#ifndef EXTEND_MATH_H
#define EXTEND_MATH_H

#include <Arduino.h>

int gcd(int a, int b); //calculates the gratest common divisor between 2 numbers[gcd]

double normalizeAngle(double a); //normalize angle to 2 * PI

//Vector arithmetics*****************************************************

//adds vectors a + b and stores the result in s
void addVectors(double ax, double ay, double bx, double by, double *sx, double *sy); //being: a = {ax, ay} ; b = {bx, by}; s = {sx, sy}
void addVectorsPolar(double ar, double aphi, double br, double bphi, double *sr, double *sphi); //being: a = {ar, aphi} ; b = {br, bphi}; s = {sr, sphi}

//END Vector arithmetics*************************************************

#endif
