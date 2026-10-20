/*
* File: storage.c
* Author: Emily Rodriguez
* Date: 10/6/206
* Provides: storage layer of program
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "vectors.h"

static vect vectors[10];
static int size = 10;

//Initialize array
void initialize() {
    for (int i = 0; i < size; i++) {
        strcpy(vectors[i].name, "null");
        vectors[i].x = 0.0;
        vectors[i].y = 0.0;
        vectors[i].z = 0.0;
    }
}

//Add a new vector to array
int addvect(vect new) {
    for (int i = 0; i < size; i++) {
        if (vectors[i].name == new.name) {
            vectors[i] = new;
            return 0;
        } else if(strcmp(vectors[i].name, "null") == 0) {
            vectors[i] = new;
            return 0;
        }
    }
    return -1;
}

//Find a vector in array
int findvect(const char* name, vect *found) {
    for (int i = 0; i < size; i++) {
        if (strcmp(vectors[i].name, name) == 0) {
            if (found != NULL) {
                *found = vectors[i];
            }
            return 0;
        }
    }
    return -1;
}

//List array
void listvects(void) {
    bool found = false;
    printf("Stored Vectors:\n");
    for (int i = 0; i < size; i++) {
        if (strcmp(vectors[i].name, "null") != 0) {
            printf("%s = (%.4f, %.4f, %.4f)\n", vectors[i].name, vectors[i].x, vectors[i].y, vectors[i].z);
            found = true;
        }
    }
    if (!found) {
        printf("No vectors currently stored.\n");
    }
}

//Clear the array
void clear() {
   initialize();
}

