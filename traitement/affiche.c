//
//  affiche.c
//  A man named Klapury
//
//  Created by Vincent Maladiere on 17/04/16.
//  Copyright © 2016 Vincent Maladiere. All rights reserved.
//

#include "affiche.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fonctions.h"
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <SDL/SDL.h>

void pauuse()
{
    int continuer = 1;
    SDL_Event event;
    
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}


void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel) {
    
    int byteperpixel = theScreen->format->BytesPerPixel;
    Uint8 *p = (Uint8*)theScreen->pixels + y * theScreen->pitch + x * byteperpixel;
    // Adress to pixel
   
    *(Uint32 *)p = pixel;
   
}


void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel) {
   
    int i;
    double x = x1 - x0;
    double y = y1 - y0;
    double length = sqrt( x*x + y*y );
    double addx = x / length;
    double addy = y / length;
    x = x0;
    y = y0;
    for ( i = 0; i < length; i += 1) {
        putpixel(Screen, x, y + 700, pixel ); //WEIGHT/2
        x += addx;
        y += addy;
    }
}
