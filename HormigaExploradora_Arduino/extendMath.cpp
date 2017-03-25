/*
* ANDRBR 2017
*
* extendMath.cpp
*
* Library designed to add some math functionality
*/

#include "extendMath.h"

//calculates the gratest common divisor between 2 numbers[gcd]
int gcd( int a, int b ){ return b == 0 ? a : gcd(b, a % b); }

//conversions between radians and degrees
double deg2rad( double a ){ return PI * a / 180; }
double rad2deg( double a ){ return 180 * a / PI; }

//normalize angle to 2 * PI
double normalizeAngle(double a){ return atan2(sin(a), cos(a)); }

//Vector arithmetics*****************************************************

//adds vectors a + b and stores the result in s
void addVectors(double ax, double ay, double bx, double by, double *sx, double *sy){
    *sx = ax + bx; //saves x magnitude
    *sy = ay + by; //saves y magnitude
}
void addVectorsPolar(double ar, double aphi, double br, double bphi, double *sr, double *sphi){
    *sr   = sqrt(pow(ar, 2) + pow(br, 2) + 2 * ar * br * cos(bphi - aphi));  //saves magnitude
    *sphi = aphi + atan2(br * sin(bphi - aphi), ar + br * cos(bphi - aphi)); //saves angle(radians)
}

//END Vector arithmetics*************************************************
