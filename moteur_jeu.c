#include <stdio.h>
#include <stdlib.h>

#define TAILLE_MAP 8


typedef struct map {
    int taille_map;
    int cases[TAILLE_MAP][TAILLE_MAP];
} map_t;

typedef struct position {
    int x;
    int y;
} pos_t;

typedef struct joueur{
    int skin;
    pos_t position_joueur;
    int nb_bombes;
    int nb_vies;
    int vitesse;
} joueur_t;

/* Fonction d'affichage de la carte */
void affichage_carte(map_t carte){
    //Programme permettant d'afficher la carte dans le terminale sous forme d'une "matrice"
    int taille_map = carte.taille_map;
    printf("Affichage d'une carte de taille %d\n", taille_map);

    //Affichage
    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            printf("%d ", carte.cases[i][j]);
        }
        printf("\n");
    }
}

/* Fonction d'acquisition de carte à partir de fichier txt*/
void map_avec_fichier (char* nom_fichier, map_t* map){

    FILE* file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    int taille_map;
    fscanf(file, "%d", &taille_map);

    int nombre_joueurs;
    fscanf(file, "%d", &nombre_joueurs);

    joueur_t joueur [nombre_joueurs];
    int x;
    int y;
    for (int k = 0; k<nombre_joueurs; k++){
        fscanf(file, "%d", &x); 
        fscanf(file, "%d", &y);

        joueur[k].position_joueur.x = x;
        joueur[k].position_joueur.y = y;
    }


    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            printf("i=%d,j=%d\n",i,j);
            fscanf(file, "%d", &map->cases[i][j]);
        }
    }

    fclose(file);
    map->taille_map = taille_map;
}


void init_joueur(int nb_joueurs, char* nom_fichier){
    
    joueur_t joueur[nb_joueurs];
    // on récupère les positions initiales des joeurs dans le fichier
    int pos_init[nb_joueurs][2];
    FILE* file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(1);
    }
    int taille_map; int nb_j;
    fscanf(file,"%d", &taille_map);
    fscanf(file,"%d", &nb_j);
    
    if(nb_joueurs!= nb_j){
        printf("Pas le bon nombre de joueurs.\n");
        exit(1);
    }
    
    for(int i=1; i<= nb_joueurs; ++i){
        
        joueur[i].skin=i;
        joueur[i].nb_bombes=2;
        joueur[i].nb_vies=5;
        joueur[i].vitesse=1;

        pos_t position_init[i];
        fscanf(file,"%d",position_init[i].x);
        fscanf(file,"%d",position_init[i].y);


        joueur[i].position_joueur= position_init[i];
    }
    fclose(file);
}
