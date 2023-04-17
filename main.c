#include <stdio.h>
#include <SDL2/SDL.h>
#include "moteur_jeu.h"


int main(){

    //Création d'une map test pour l'afficher
    extern int taille_map;

    //Initialisation de toutes les cases à 1
    int cases[taille_map][taille_map];
    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            cases[i][j] = 1;
        }
    }


    //Initialisation de la carte avec les cases du tableau
    map_t carte ;
    map_avec_fichier ("carte_1.txt", &carte);

    affichage_carte(carte);

    affichage_jeu(carte);

    return 0;
}
