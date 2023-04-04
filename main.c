#include <stdio.h>
//#include "moteur_jeu.h"
#define TAILLE_MAP 8

//A supprimer : doit être dans moteur_jeu.c
typedef struct map {
    int taille_map;
    int cases[TAILLE_MAP][TAILLE_MAP];
} map_t;

void affichage_carte(map_t carte){
    //Programme permettant d'afficher la carte dans le terminale sous forme d'une "matrice"
    int taille_map = carte.taille_map;
    printf("Affichage d'une carte de taille %d\n", taille_map);

    int valeurs_case[taille_map][taille_map];
    //Copie des valeurs de la carte dans le tableau "valeurs_case"
    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            valeurs_case[i][j] = carte.cases[i][j];
        }
    }

    //Parcours de toute la carte et affichage de chacun des éléments du tableau
    for (int i = 0; i < taille_map; i++){
        for (int j = 0; j < taille_map; j++){
            printf("%d ", valeurs_case[i][j]);
        }
        printf("\n");
    }
}
//Suppression terminée

int main(){

    //Création d'une map test pour l'afficher
    int taille_map = 8;
    int cases[taille_map][taille_map];
    for (int i = 0; i < taille_map; i++){
        for (int j = 0; j < taille_map; j++){
            cases[i][j] = 0;
        }
    }

    map_t carte = {taille_map, {{0}}};

    affichage_carte(carte);

    return 0;
}
