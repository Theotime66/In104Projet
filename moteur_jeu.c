#include <stdio.h>

typedef struct map{
    int taille_map;
    int cases[taille_map][taille_map];
};


void affichage_carte(struct map){
    //Programme permettant d'afficher la carte dans le terminale sous forme d'une "matrice"
    int taille_map = map.taille_map;
    int valeurs_case [taille_map][taille_map] = map.cases;

    //Parcourt de toute la carte et affichage de chacun des éléments du tableau
    for (int i = 0; i<taille_map; i++){
        for (int j = 0; j<taille_map; j++){
            int valeur_case = map.case
            printf("%d ",valeur_case[i][j]);
        }
        printf("\n");
    }
}