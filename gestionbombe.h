#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "moteur_jeu.h"
#include "pile.h"

void explosion_bombe(map_t* map, bomb_t* bombe,joueur_t* joueur , pile_t* cases_explosions_i, pile_t* cases_explosions_j);

int a_explose (bomb_t bombe, int temps_pose_bombe, int temps_courant);

void nettoyage_cases_explosion(map_t* map, bomb_t bombe, pile_t cases_explosions_i, pile_t cases_explosions_j);

bomb_t poser_bombe(joueur_t* joueur, map_t* map);