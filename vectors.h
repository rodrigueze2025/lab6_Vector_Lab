/*
* File: vectors.h
* Author: Emily Rodriguez
* Date: 10/6/206
* Provides: vector header file
*/

#ifndef VECTORS_H
#define VECTORS_H

#define MAX_VECTORS 10

//vector struct
typedef struct {
    char name[32];
    double x;
    double y;
    double z;
} vect;

//math ops
vect add(vect a, vect b);
vect sub(vect a, vect b);
vect multi(vect a, double scalar);

//storage
void initialize();
int addvect(vect new);
int findvect(const char *name, vect *found);
void listvects();
void clear();

//user interface
void printhelp(const char *progname);
void runUI();

#endif

