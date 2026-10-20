/*
*
*/

#include "vectors.h"

//addition
vect add(vect a, vect b) {
    vect sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    sum.z = a.z + b.z;
    return sum;
}

//subtraction
vect sub(vect a, vect b) {
    vect dif;
    dif.x = a.x - b.x;
    dif.y = a.y - b.y;
    dif.z = a.z - b.z;
    return dif;
}

//multiplication
vect multi(vect a, double b) {
    vect prod;
    prod.x = a.x*b;
    prod.y = a.y*b;
    prod.z = a.z*b;
    return prod;
}