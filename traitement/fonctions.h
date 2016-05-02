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
void fosup(frame Zsmoothed, frame Z, int taille );
int processing_init(frame X, frame Lmax,frame Npow, int taille, double threshold,int k0,int k1);
int iteration_checking(frame X, frame Lmaxi,frame Npow, int taille,double treshold, int k0, int k1);
int F0fromL(double* L, int taille);
double mean(double* T, int taille);
#endif /* fonctions_h */
