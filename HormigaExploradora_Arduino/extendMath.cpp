/*
* ANDRBR 2017
*
* extendMath.cpp
*
* Library designed to add some math functionality
*/

#include "extendMath.h"

long gcd(long a, long b){ return b == 0 ? a : gcd(b, a % b); } //calculates the gratest common divider between 2 numbers[gcd]
