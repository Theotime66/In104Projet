#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define TAILLE_MAP 14

typedef struct map {
    int taille_map;
    int cases[TAILLE_MAP][TAILLE_MAP];
} map_t;

typedef struct position_xy {
    int x;
    int y;
} pos_xy_t;

typedef struct position_ij {
    int i;
    int j;
} pos_ij_t;

typedef struct joueur{
    int skin;
    pos_xy_t position_joueur;
    int nb_bombes;
    int nb_vies;
    int vitesse;
} joueur_t;

typedef struct bombe{
    pos_xy_t pos_bombe;
    SDL_TimerID timer_id; // à voir pas sur de continuer à utiliser SDL_TimeID je suis pas sur du focntionnement du callback
    int rayon;
} bomb_t;   

struct param{ // structure nécessaire pour l'utilisation de la focntion callback 
    map_t map;
    joueur_t joueur;
    int nb_joueur;
    bomb_t bombe; 
    bomb_t bombes; 
};

void poser_bombe(joueur_t* joueur, map_t* map, bomb_t* bombes);

Uint32 bombe_explosion_callback(Uint32 SDL_TimerCallback,void* parametres);

void explosion_bombe(map_t* map, bomb_t* bombe,joueur_t* joueur);
