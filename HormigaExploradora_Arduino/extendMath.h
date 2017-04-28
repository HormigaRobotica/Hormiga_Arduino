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

long gcd(long a, long b); //calculates the gratest common divider between 2 numbers[gcd]

template <typename T> T normalizeAngle(T a){ return atan2(sin(a), cos(a)); } //normalize angle to 2 * PI

//Vector arithmetics*****************************************************

//adds vectors a + b and stores the result in s
template <typename T> void addVectors(T ax, T ay, T bx, T by, T *sx, T *sy){ //being: a = {ax, ay} ; b = {bx, by}; s = {sx, sy}
	*sx = ax + bx; //saves x magnitude
	*sy = ay + by; //saves y magnitude
}

template <typename T> void addVectorsPolar(T ar, T aphi, T br, T bphi, T *sr, T *sphi){//being: a = {ar, aphi} ; b = {br, bphi}; s = {sr, sphi}
	*sr   = sqrt(pow(ar, 2) + pow(br, 2) + 2 * ar * br * cos(bphi - aphi));  //saves magnitude
	*sphi = aphi + atan2(br * sin(bphi - aphi), ar + br * cos(bphi - aphi)); //saves angle(radians)
}

//END Vector arithmetics*************************************************

//Coordinate system transformations**************************************
#define CARTESIAN_SYS   0
#define CYLINDRICAL_SYS 1
#define SPHERICAL_SYS   2

template <typename T> void toCartesian(T a, T b, T c, T* x, T* y, T* z, byte from){
	switch(from){
		case CYLINDRICAL_SYS:
			*x = a * cos(b);
			*y = a * sin(b);
			*z = c;
			break;
		case SPHERICAL_SYS:
			*x = a * cos(c) * sin(b);
			*y = a * sin(c) * sin(b);
			*z = a * cos(b);
			break;
	};
}

template <typename T> void toCartesian(T a, T b, T* x, T* y){
	*x = a * cos(b);
	*y = a * sin(b);
}

template <typename T> void toPolar(T a, T b, T* r, T* theta){
	*r = sqrt((a * a) + (b * b));
	*theta = atan2(b,a);
}

template <typename T> void toCylindrical(T a, T b, T c, T* r, T* theta, T* z, byte from){
	switch(from){
		case CARTESIAN_SYS:
			*r = sqrt((a * a) + (b * b));
			*theta = atan2(b,a);
			*z = c;
			break;
		case SPHERICAL_SYS:
			*r = a * sin(b);
			*theta = c;
			*z = a * cos(b);
			break;
	};
}

template <typename T> void toSpherical(T a, T b, T c, T* rho, T* phi, T* theta, byte from){
	switch(from){
		case CYLINDRICAL_SYS:
			*rho = sqrt((a * a) + (c * c));
			*phi = atan2(a,c);
			*theta = b;
			break;
		case CARTESIAN_SYS:
			*rho = sqrt((a * a) + (b * b) + (c * c));
			*phi = atan2(sqrt((a * a) + (b * b)), c);
			*theta = atan2(b,a);
			break;
	};
}

//END Coordinate system transformations**********************************

#endif
