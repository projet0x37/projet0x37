//
//  affiche.h
//  A man named Klapury
//
//  Created by Vincent Maladiere on 17/04/16.
//  Copyright © 2016 Vincent Maladiere. All rights reserved.
//

#ifndef affiche_h
#define affiche_h

#include <stdio.h>
#include <signal.h>
#include <SDL/SDL.h>

void pauuse();
void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel);
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel);

#endif /* affiche_h */
