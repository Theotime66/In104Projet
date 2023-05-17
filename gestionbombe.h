#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "moteur_jeu.h"

typedef struct param{
    map_t map;
    joueur_t joueur;
    bomb_t bombe; 
}param_t;


void poser_bombe(joueur_t* joueur, map_t* map, bomb_t* bombes);

Uint32 bombe_explosion_callback(Uint32 SDL_TimerCallback,void* parametres);

void explosion_bombe(map_t* map, bomb_t* bombe,joueur_t* joueur);
