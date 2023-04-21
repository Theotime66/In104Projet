#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "generic_queue.h"

// Utiliser un tableau d'entiers pour stocker les codes de touche autorisés
const int CHAR_AUTORISES[] = {SDLK_q, SDLK_s, SDLK_d, SDLK_z, SDLK_l, SDLK_CAPSLOCK, SDLK_LSHIFT, SDLK_SEMICOLON, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN};
// Déterminer la taille du tableau en divisant la taille totale par la taille d'un élément
const int TAILLE_CHAR_AUTORISES = sizeof(CHAR_AUTORISES) / sizeof(CHAR_AUTORISES[0]);

generic_queue_t commande(int win) {
    // Créer une nouvelle file
    generic_queue_t *fifo = generic_queue_init(); // peut être que la fifo doit être en argument est initialisée dans le main

    SDL_Event event;
    while (!win) {
        // Gérer les événements
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    win = 1;
                    break;
                case SDL_KEYDOWN:
                    // Vérifier si la touche est autorisée en parcourant le tableau
                    int touche_autorisee = 0;
                    for (int i = 0; i < TAILLE_CHAR_AUTORISES; i++) {
                        if (event.key.keysym.sym == CHAR_AUTORISES[i]) {
                            touche_autorisee = 1;
                            break;
                        }
                    }
                    // Si la touche n'est pas autorisée, ajouter 'error' à la file
                    if (!touche_autorisee) {
                        generic_queue_enqueue(fifo, "error");
                    }
                    // Sinon, ajouter le code de touche à la file
                    else {
                        generic_queue_enqueue(fifo, &event.key.keysym.sym);
                    }
                    break;
            }
        }
    }
    
    return fifo;
    // Détruire la file 
    generic_queue_destroy(fifo);
    
}
