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

    int taille_map;

    FILE* file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(1);
    }
    fscanf(file, "%d", &taille_map);

    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            fscanf(file, "%d", &map->cases[i][j]);
        }
    }

    fclose(file);
    map->taille_map = taille_map;
}

void init_joueur(int nb_joueurs, char* nom_fichier){
    // on récupère les positions initiales des joeurs dans le fichier
    joueur_t joueur[nb_joueurs];

    for(int i=1, i<= nb_joueurs, ++i){
        
        joueur[i]->skin=i;
        joueur[i]->nb_bombes=2;
        joueur[i]->nb_vies=5;
        joueur[i]->vitesse=1;

        pos_t position_init_i;

        position_init[i]->x=position_init[i][1];
        position_init[i]->y=position_init[i][2];

        joueur[i]->position_joueur= position_init_i;


    }
}