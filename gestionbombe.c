#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "moteur_jeu.h"
#include "pile.h"

void explosion_bombe(map_t* map, bomb_t* bombe,joueur_t* joueur , pile_t* cases_explosions_i, pile_t* cases_explosions_j){
    //map_t* map = *map2;
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
        //Stockage des indices des explosions
        empile(cases_explosions_i, i);
        empile(cases_explosions_j, j);
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
        //Stockage des indices des explosions
        empile(cases_explosions_i, i);
        empile(cases_explosions_j, j);
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
        //Stockage des indices des explosions
        empile(cases_explosions_i, i);
        empile(cases_explosions_j, j);
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
        //Stockage des indices des explosions
        empile(cases_explosions_i, i);
        empile(cases_explosions_j, j);
    }
}

int a_explose (bomb_t bombe, int temps_pose_bombe, int temps_courant){
    int temps_explosion_bombe = bombe.temps_explosion;

    if(temps_courant > temps_pose_bombe + temps_explosion_bombe){
        return 1;
    }
    return 0;
}

void nettoyage_cases_explosion(map_t* map, bomb_t bombe, pile_t cases_explosions_i, pile_t cases_explosions_j){
    //Fonction prenant comme argument 2 piles correspondant aux coordonnées des explosions
    //Fonction permettant de supprimer les cases explosées ie de remplacer dans la matrice map les 45 par des 0
    int i;
    int j;

    //Fonctionne uniquement si les piles des coordonnées i et j sont bien remplies, ie de meme taille et dans le même ordre.
    while (vide(&cases_explosions_i) != -1){
        depile(&cases_explosions_i, &i);
        depile(&cases_explosions_j, &j);

        map->cases[i][j] = 0;
    }
}

// Timing may be inexact due to OS scheduling. Be sure to note the current time with SDL_GetTicks( ) or SDL_GetPerformanceCounter( ) 
// in case your callback needs to adjust for variances.
// fonction qui permet de poser une bombe si possible et de l'initialiser 
// Il faut mettre absolument dans le main SDL_Init(SDL_INIT_TIMER) pour que SDL_AddTimer fonctionne 
bomb_t poser_bombe(joueur_t* joueur, map_t* map){

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
        n_bombe.rayon = RAYON_EXPLOSION;
        n_bombe.temps_explosion = TPS_EXPLOSION;
        n_bombe.joueur_poseur_bombe = *joueur;

        ////Initialise la pile stockant les endroits où la bombe à explosé
        //pile_t cases_explosions_i;
        //pile_t cases_explosions_j;

        // frameTimeBombe = SDL_GetTicks() - frameStartBombe;
        // while (frameTimeBombe < TPS_EXPLOSION){
        //     frameTimeBombe = SDL_GetTicks() - frameStartBombe;
        //     printf("frameTimeBombe = %d\n",frameTimeBombe);
        // }
        //SDL_Delay(TPS_EXPLOSION);
        
        //while(SDL_GetTicks()-frameStartBombe < TPS_EXPLOSION){
        //}
        //explosion_bombe(&map, &n_bombe, joueur, &cases_explosions_i, &cases_explosions_j);

        return n_bombe;
    }
}
