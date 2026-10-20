/*
* File: main.c
* Author: Emily Rodriguez
* Date: 10/6/206
* Provides: main file
*/

#include <stdio.h>
#include <string.h>
#include "vectors.h"



int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printhelp(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[1]);
            printhelp(argv[0]);
            return 1;
        }
    }

    // Hand off control to the user interface layer
    runUI();

    return 0;
}
