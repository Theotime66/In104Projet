#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "moteur_jeu.h"

void explosion_bombe(map_t** map2, bomb_t* bombe,joueur_t* joueur ){
    map_t* map = *map2;
    int i = bombe->pos_bombe.j;
    int j = bombe->pos_bombe.i;
    printf("i=%d,j=%d\n",i,j);
    int rayon = bombe->rayon;

    // on met à jour la matrice
    map->cases[i][j]=0;

    // disjonction de cas pour les directions
    for(int k=j+1; k< j+rayon; k++){
        if(map->cases[i][k]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[i][k]<11 && map->cases[i][k]>0){
            map->cases[i][k] = 0; // on abaisse le niveau du mur destructible
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
    }

    for(int k=j-1;  k > j-rayon; k--){
        if(map->cases[i][k]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[i][k]<11 && map->cases[i][k]>0){
            map->cases[i][k] = 0; // on abaisse le niveau du mur destructible
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
    }

    for(int k=i+1; k<i+rayon; k++){
        if(map->cases[k][j]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[k][j]<11 && map->cases[k][j]>0){
            map->cases[k][j] = 0; // on abaisse le niveau du mur destructible
            break; 
        }
        if(map->cases[k][j]<39 && map->cases[k][j]>29){
            int l=0;// il faut chercher le joeur en fonction de son skin pour abaisser sa vie
            while(joueur[l].skin != map->cases[k][j]){
                l++;}
            joueur[l].nb_vies --;
                
            // renvoyer dans la liste animation perte de vie faire l'animation en SDL  
        }
        map->cases[k][j]=45;
        }


    for(int k=i-1; k>i-rayon; k--){
        if(map->cases[k][j]==11){
            break; //on arrête l'explosion car mur indestructible
        }

        if(map->cases[k][j]<11 && map->cases[k][j]>0){
            map->cases[k][j] = 0; // on abaisse le niveau du mur destructible
            break; 
        }
        if(map->cases[k][j]<39 && map->cases[k][j]>29){
            int l=0;// il faut chercher le joeur en fonction de son skin pour abaisser sa vie
            while(joueur[l].skin != map->cases[k][i]){
                l++;}
            joueur[l].nb_vies --;
        }
        map->cases[k][j]=45;

    }
    Uint32 frameStartFire = SDL_GetTicks();
     while(SDL_GetTicks()-frameStartFire < 500){
        }
    // fifo des flammes à 0 
}


// Timing may be inexact due to OS scheduling. Be sure to note the current time with SDL_GetTicks( ) or SDL_GetPerformanceCounter( ) 
// in case your callback needs to adjust for variances.
// fonction qui permet de poser une bombe si possible et de l'initialiser 
// Il faut mettre absolument dans le main SDL_Init(SDL_INIT_TIMER) pour que SDL_AddTimer fonctionne 
void poser_bombe(joueur_t* joueur, map_t* map){

    Uint32 frameStartBombe = SDL_GetTicks();

    int TPS_EXPLOSION=3000; //le Uint32 est pour faire focntionner SDL_AddTimer 
    int RAYON_EXPLOSION=3;  // pour le moment des cte pourra être changer si différents niveaux de bombes

    if(joueur->nb_bombes>0){
        int x = joueur->position_joueur.x;
        int y = joueur->position_joueur.y;
        pos_xy_t XY_Joueur = {x,y};
        pos_ij_t IJ_Joueur = transformation_xy_ij(XY_Joueur,0,0);

        map->cases[IJ_Joueur.j][IJ_Joueur.i] = 41; // on change ce qui est dans la case 
        joueur->nb_bombes--; // on abaisse le nombre de bombe du joueur 
        
        // on initialise la nouvelle bombe 
        bomb_t n_bombe;
        n_bombe.pos_bombe.i = IJ_Joueur.i;
        n_bombe.pos_bombe.j = IJ_Joueur.j;
        n_bombe.rayon= RAYON_EXPLOSION;
        n_bombe.temps_explosion = TPS_EXPLOSION;

        // frameTimeBombe = SDL_GetTicks() - frameStartBombe;
        // while (frameTimeBombe < TPS_EXPLOSION){
        //     frameTimeBombe = SDL_GetTicks() - frameStartBombe;
        //     printf("frameTimeBombe = %d\n",frameTimeBombe);
        // }
        //SDL_Delay(TPS_EXPLOSION);
        while(SDL_GetTicks()-frameStartBombe < TPS_EXPLOSION){
        }
        explosion_bombe(&map, &n_bombe, joueur);
    }
}
