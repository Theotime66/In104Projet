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



void explosion_bombe(map_t* map, bomb_t* bombe,joueur_t* joueur ){
    int i = bombe->pos_bombe.x;
    int j = bombe->pos_bombe.y;
    int rayon = bombe->rayon;

    // on met à jour la matrice
    map->cases[i][j]=0;

    // disjonction de cas pour les directions
    for(int k=j+1; k< j+rayon; k++){
        if(map->cases[i][k]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[i][k]<11 && map->cases[i][k]>0){
            map->cases[i][k]=0; // on abaisse le niveau du mur destructible
            break; 
        }
        if(map->cases[i][k]<40 && map->cases[i][k]>29){
            int l=0;// il faut chercher le joeur en fonction de son skin pour abaisser sa vie
            while(joueur[l].skin != map->cases[i][k]){
                l++;}
            joueur[l].nb_vies --;
                
            // renvoyer dans la liste animation perte de vie faire l'animation en SDL  
        }
        map->cases[i][k]=45;
        // comment faire apparaitre les flammes => avec call back 
    }

    for(int k=j-1;  k > j-rayon; k--){
        if(map->cases[i][k]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[i][k]<11 && map->cases[i][k]>0){
            map->cases[i][k]--; // on abaisse le niveau du mur destructible
            break; 
        }
        if(map->cases[i][k]<39 && map->cases[i][k]>29){
            int l=0;// il faut chercher le joeur en fonction de son skin pour abaisser sa vie
            while(joueur[l].skin != map->cases[i][k]){
                l++;}
            joueur[l].nb_vies --;
                
            // renvoyer dans la liste animation perte de vie faire l'animation en SDL  
        }
        map->cases[i][k]=45;
        // comment faire apparaitre les flammes => avec call back 
    }

    for(int k=i+1; k<i+rayon; k++){
        if(map->cases[i][k]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[k][j]<11 && map->cases[k][j]>0){
            map->cases[k][j]--; // on abaisse le niveau du mur destructible
            break; 
        }
        if(map->cases[k][j]<39 && map->cases[k][j]>29){
            int l=0;// il faut chercher le joeur en fonction de son skin pour abaisser sa vie
            while(joueur[l].skin != map->cases[i][k]){
                l++;}
            joueur[l].nb_vies --;
                
            // renvoyer dans la liste animation perte de vie faire l'animation en SDL  
        }
        map->cases[i][k]=45;
        }


    for(int k=i-1; k>i-rayon; k--){
        if(map->cases[i][k]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[k][j]<11 && map->cases[k][j]>0){
            map->cases[k][j]--; // on abaisse le niveau du mur destructible
            break; 
        }
        if(map->cases[k][j]<39 && map->cases[k][j]>29){
            int l=0;// il faut chercher le joeur en fonction de son skin pour abaisser sa vie
            while(joueur[l].skin != map->cases[i][k]){
                l++;}
            joueur[l].nb_vies --;
        }
        // comment faire apparaitre les flammes => avec call back 
    }
}

unsigned int bombe_explosion_callback(unsigned int interval,void* parametres){    
    
    // On récupère les informations nécessaires pour l'explosion de la bombe
    param_t * param = (param_t*)parametres;

    bomb_t bombe = param->bombe;
    map_t map = param->map;
    joueur_t joueurs = param->joueur;

/*    bomb_t* bombe = &(params->bombe);
    bomb_t* l_bombes = &(params->bombes);
    map_t* map = &(params->map);
    int nb_joueurs = params->nb_joueur;
    joueur_t* joueurs = &(params->joueur);*/ 
    
    // On crée une explosion/destruction centrée sur la position de la bombe
    explosion_bombe(&map,&bombe,&joueurs);
    
    // On retourne 0 pour que le timer ne se répète pas
    return 0;
    //SDL_RemoveTimer();
}


// Timing may be inexact due to OS scheduling. Be sure to note the current time with SDL_GetTicks( ) or SDL_GetPerformanceCounter( ) 
// in case your callback needs to adjust for variances.
// fonction qui permet de poser une bombe si possible et de l'initialiser 
// Il faut mettre absolument dans le main SDL_Init(SDL_INIT_TIMER) pour que SDL_AddTimer fonctionne 
void poser_bombe(joueur_t* joueur, map_t* map, bomb_t* bombes){

    Uint32 TPS_EXPLOSION=3000; //le Uint32 est pour faire focntionner SDL_AddTimer 
    int RAYON_EXPLOSION=3;  // pour le moment des cte pourra être changer si différents niveaux de bombes

    if(joueur->nb_bombes>0){
        map->cases[joueur->position_joueur.x][joueur->position_joueur.y]=41; // on change ce qui est dans la case 
        joueur->nb_bombes--; // on abaisse le nombre de bombe du joueur 
        
        // on initialise la nouvelle bombe 
        bomb_t n_bombe;
        n_bombe.pos_bombe.x=joueur->position_joueur.x;
        n_bombe.pos_bombe.y=joueur->position_joueur.y;


        param_t param_bombe_explosion_callback;
        param_bombe_explosion_callback.map = *map;
        param_bombe_explosion_callback.joueur = *joueur;
        param_bombe_explosion_callback.bombe = n_bombe;

        n_bombe.timer_id= SDL_AddTimer(TPS_EXPLOSION, bombe_explosion_callback, &param_bombe_explosion_callback); 
        n_bombe.rayon= RAYON_EXPLOSION;
        
    }
}
