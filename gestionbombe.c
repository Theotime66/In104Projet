#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

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
struct param{
    map_t map;
    joueur_t joueur;
    int nb_joueur;
    bomb_t bombe; 
    bomb_t bombes; 
};

// Timing may be inexact due to OS scheduling. Be sure to note the current time with SDL_GetTicks( ) or SDL_GetPerformanceCounter( ) 
// in case your callback needs to adjust for variances.
// fonction qui permet de poser une bombe si possible et de l'initialiser 
// Il faut mettre absolument dans le main SDL_Init(SDL_INIT_TIMER) pour que SDL_AddTimer fonctionne 
void poser_bombe(joueur_t* joueur, map_t* map, bomb_t* bombes,int* nb_bombes,int touche_pressee){

    Uint32 TPS_EXPLOSION=3000; //le Uint32 est pour faire focntionner SDL_AddTimer 
    int RAYON_EXPLOSION=3;  // pour le moment des cte pourra être changer si différents niveaux de bombes

    if(joueur->nb_bombes>0){
        map->cases[joueur->position_joueur.x][joueur->position_joueur.y]=41; // on change ce qui est dans la case 
        joueur->nb_bombes--; // on abaisse le nombre de bombe du joueur 
        
        // on initialise la nouvelle bombe 
        bomb_t n_bombe;
        n_bombe.pos_bombe.x=joueur->position_joueur.x;
        n_bombe.pos_bombe.y=joueur->position_joueur.y;
        
        struct param param_bombe_explosion_callback;
        param_bombe_explosion_callback.map = *map;
        param_bombe_explosion_callback.joueur = *joueur;
        param_bombe_explosion_callback.nb_joueur = nb_joueur; // Modifier avec le nombre de joueurs
        param_bombe_explosion_callback.bombe = n_bombe;
        param_bombe_explosion_callback.bombes = bombes;
        
        n_bombe.timer_id= SDL_AddTimer(TPS_EXPLOSION,bombe_explosion_callback, param_bombe_explosion_callback); 
        n_bombe.rayon= RAYON_EXPLOSION;
        
        bombes[*nb_bombes] = n_bombe; // on ajoute la nouvelle bombe à la liste des bombes
        (*nb_bombes)++; // on augmente le nb de bombe sur le terrain ? Il y a t il un max ? 
    }
}

Uint32 bombe_explosion_callback(Uint32 SDL_TimerCallback,void* parametres){    
    
    // On récupère les informations nécessaires pour l'explosion de la bombe
    bomb_t* bombe = parametres->n_bombe;
    bomb_t* l_bombes = parametres->bombes;
    map_t* map = parametres->map;
    int nb_joueurs = parametres->nb_joueur;
    joueur_t* joueurs = parametres->joueur;
     param_t* params = (param_t*)parametres;

/*    bomb_t* bombe = &(params->bombe);
    bomb_t* l_bombes = &(params->bombes);
    map_t* map = &(params->map);
    int nb_joueurs = params->nb_joueur;
    joueur_t* joueurs = &(params->joueur);*/ 
    
    // On crée une explosion/destruction centrée sur la position de la bombe
    explosion_bombe(map,bombe,nb_joueurs,joueurs);
    
    // On retire la bombe de la liste des bombes
    int i=0;
    while(l_bombes[i].timer_id !=bombe->timer_id){
        i++;
    }
    for(int k=i; k<nb_joueurs-1 ;k++){
                l_bombes[k]=l_bombes[k+1]; // on décale tous les bombes à partir i ème, cela permet d'éliminer la bombe explosée
            }
    // On retourne 0 pour que le timer ne se répète pas
    return 0;
    //SDL_RemoveTimer();
}

void explosion_bombe(map_t* map, bomb_t* bombe,int nb_joueurs,joueur_t* joueur ){
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
                l++1;}
            joueur[l].nb_vies --1;
                
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
                l++1;}
            joueur[l].nb_vies --1;
                
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
                l++1;}
            joueur[l].nb_vies --1;
                
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
                l++1;}
            joueur[l].nb_vies --1;
        }
            // renvoyer dans la liste animation perte de vie faire l'animation en SDL  
        
        map->cases[i][k]=45;
        }
}
