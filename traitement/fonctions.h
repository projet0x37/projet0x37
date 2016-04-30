//
//  fonctions.h
//  A man named Klapury
//
//  Created by Vincent Maladiere on 15/04/16.
//  Copyright © 2016 Vincent Maladiere. All rights reserved.
//

#ifndef fonctions_h
#define fonctions_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double frand_a_b(double a, double b);
double * Y_extraction(double * X, int taille);
double * moving_average(double * Y, double largeur, int taille);
double * Z_calc(double * Y, double * N, int taille);
#endif /* fonctions_h */
