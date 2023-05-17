#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "moteur_jeu.h"

void explosion_bombe(map_t** map2, bomb_t* bombe,joueur_t* joueur );

void poser_bombe(joueur_t* joueur, map_t* map);