#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <stdio.h>
#include <stdlib.h>
#include "assemble.h" 

char return_alphabet(int t);
int substitution_encode(char*ibuffer, int num, char*output);
int substitution_decode(char*ibuffer, int num, char*output);

#endif
