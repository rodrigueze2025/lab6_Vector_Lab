/*
* File: ui.c
* Author: Emily Rodriguez
* Date: 10/6/206
* Provides: User interface layer
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vectors.h"

//Help menut
void printhelp(const char *progname) {
    printf("Vector Calculator Help:\n");
    printf("    Usage: %s [-h]\n", progname);
    printf("    Commands and Expressions:\n");
    printf("        quit - exit the program\n");
    printf("        clear - clear all stored vectors\n");
    printf("        list - lists all the stored vectors\n");
    printf("        varname - displays the value of named vector\n");
    printf("        varname = x, y, z - create a new vector (SPACES REQUIRED)\n");
    printf("        var1 + var2 - adds two vectors (SPACES REQUIRED)\n");
    printf("        var1 - var2 - subtracts two vectors (SPACES REQUIRED)\n");
    printf("        var1 * num OR num * var1 - multiples vector by number (SPACES REQUIRED)\n");
    printf("        result = var1 +/- var2 - performs indicated operation and stores it in result vector\n");
}

//Helper to trim trailing/leading spaces or newlines
static void trim_string(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

//User interface
void runUI(void) {
    char line[256];
    initialize();

    printf("Welcome to Vector Calculator!\n");
    while (1) {
        printf(">> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        trim_string(line);

        if (strlen(line) == 0) {
            continue;
        }

        if (strcmp(line, "quit") == 0) {
            break;
        } else if (strcmp(line, "clear") == 0) {
            clear();
            printf("Storage cleared.\n");
            continue;
        } else if (strcmp(line, "list") == 0) {
            listvects();
            continue;
        }

        //Tokenize line using space separation
        //Copy line for tokenization
        char line_copy[256];
        strcpy(line_copy, line);

        char *tokens[10];
        int t_count = 0;
        char *token = strtok(line_copy, " ");
        while (token != NULL && t_count < 10) {
            tokens[t_count++] = token;
            token = strtok(NULL, " ");
        }

        if (t_count == 0) continue;

        //Case 1: Display vector -> varname
        if (t_count == 1) {
            vect v;
            if (findvect(tokens[0], &v) == 0) {
                printf("%s = (%.4f, %.4f, %.4f)\n", v.name, v.x, v.y, v.z);
            } else {
                printf("Error: Vector '%s' does not exist.\n", tokens[0]);
            }
        }
        //Case 2: Operation on two existing vectors -> var1 + var2  or var1 - var2
        else if (t_count == 3 && (strcmp(tokens[1], "+") == 0 || strcmp(tokens[1], "-") == 0)) {
            vect a, b;
            if (findvect(tokens[0], &a) != 0) {
                printf("Error: Vector '%s' does not exist.\n", tokens[0]);
                continue;
            }
            if (findvect(tokens[2], &b) != 0) {
                printf("Error: Vector '%s' does not exist.\n", tokens[2]);
                continue;
            }

            vect res;
            if (strcmp(tokens[1], "+") == 0) {
                res = add(a, b);
            } else {
                res = sub(a, b);
            }
            printf("Result = (%.4f, %.4f, %.4f)\n", res.x, res.y, res.z);
        }
        //Case 3: Scalar multiplication -> var1 * num  or  num * var1
        else if (t_count == 3 && strcmp(tokens[1], "*") == 0) {
            vect v;
            double scalar = 0.0;

            if (findvect(tokens[0], &v) == 0) {
                scalar = atof(tokens[2]);
            } else if (findvect(tokens[2], &v) == 0) {
                scalar = atof(tokens[0]);
            } else {
                printf("Error: Vector not found in expression.\n");
                continue;
            }

            vect res = multi(v, scalar);
            printf("Result = (%.4f, %.4f, %.4f)\n", res.x, res.y, res.z);
        }
        //Case 4: Assignment -> varname = x, y, z 
        else if (t_count >= 3 && strcmp(tokens[1], "=") == 0 && t_count <= 6) {
            char varname[50];
            strcpy(varname, tokens[0]);

            char *eq_ptr = strchr(line, '=');
            if (!eq_ptr) {
                printf("Error: Invalid assignment expression.\n");
                continue;
            }
            eq_ptr++; 

            double x = 0.0, y = 0.0, z = 0.0;
            int scanned = sscanf(eq_ptr, "%lf , %lf , %lf", &x, &y, &z);
            if (scanned < 2) {
                scanned = sscanf(eq_ptr, "%lf %lf %lf", &x, &y, &z);
            }

            if (scanned < 2) {
                printf("Error: Invalid vector values format. Use x, y, z or x y z.\n");
                continue;
            }
            if (scanned == 2) {
                z = 0.0; // Optional z component default
            }

            vect new_v;
            strcpy(new_v.name, varname);
            new_v.x = x;
            new_v.y = y;
            new_v.z = z;

            if (addvect(new_v) != 0) {
                printf("Error: Memory full! Maximum of %d vectors reached. Type 'clear' to reset storage.\n", MAX_VECTORS);
            } else {
                printf("Stored: %s = (%.4f, %.4f, %.4f)\n", new_v.name, new_v.x, new_v.y, new_v.z);
            }
        }
        //Case 5: Operation + assignment -> result = var1 + var2 (or - or *)
        else if (t_count >= 5 && strcmp(tokens[1], "=") == 0) {
            char res_name[50];
            strcpy(res_name, tokens[0]);

            vect a, b, res;
            int success = 0;

            if (t_count == 5 && (strcmp(tokens[3], "+") == 0 || strcmp(tokens[3], "-") == 0)) {
                if (findvect(tokens[2], &a) == 0 && findvect(tokens[4], &b) == 0) {
                    if (strcmp(tokens[3], "+") == 0) {
                        res = add(a, b);
                    } else {
                        res = sub(a, b);
                    }
                    success = 1;
                }
            } else if (t_count == 5 && strcmp(tokens[3], "*") == 0) {
                double scalar = 0.0;
                if (findvect(tokens[2], &a) == 0) {
                    scalar = atof(tokens[4]);
                    res = multi(a, scalar);
                    success = 1;
                } else if (findvect(tokens[4], &a) == 0) {
                    scalar = atof(tokens[2]);
                    res = multi(a, scalar);
                    success = 1;
                }
            }

            if (success) {
                strcpy(res.name, res_name);
                if (addvect(res) != 0) {
                    printf("Error: Memory full! Cannot store result vector.\n");
                } else {
                    printf("%s = (%.4f, %.4f, %.4f)\n", res.name, res.x, res.y, res.z);
                }
            } else {
                printf("Error: Invalid operation or missing vectors in expression.\n");
            }
        } else {
            printf("Error: Unrecognized command or expression syntax. Type '-h' for help.\n");
        }

    }
}



