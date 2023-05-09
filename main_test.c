#include <stdio.h>
#include <SDL2/SDL.h>

// Compilation : gcc -o jeu_video main_test.c -lSDL2

const int WINDOW_WIDTH = 994;
const int WINDOW_HEIGHT = 994;
const int PLAYER_WIDTH = 71;
const int PLAYER_HEIGHT = 71;

int main(int argc, char* args[]) {
    //Initialisation de cases
    //Création d'une map test pour l'afficher
    int taille_map = 14;

    //Initialisation de toutes les cases à 1
    int cases[taille_map][taille_map];
    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            cases[i][j] = 1;
        }
    }


void affichage_jeu(map_t carte){
    //Initialisation de cases
    int taille_map = carte.taille_map;


    // Initialiser SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Créer la fenêtre
    SDL_Window* window = SDL_CreateWindow("Jeu vidéo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    // Créer le rendu de la fenêtre
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger l'image du joueur avec transparence
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    SDL_SetColorKey(playerSurface, SDL_TRUE, SDL_MapRGB(playerSurface->format, 255, 0, 255));
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    // Charger les images des cases
    //SDL_Surface* grassSurface = SDL_LoadBMP("texture_herbe.bmp");
    //SDL_Texture* grassTexture = SDL_CreateTextureFromSurface(renderer, grassSurface);
    SDL_Surface* wallSurface = SDL_LoadBMP("texture_mur.bmp");
    SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);

    
    // Charger l'image de l'arrière-plan
    SDL_Surface* backgroundSurface = SDL_LoadBMP("background.bmp");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_Rect backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    // Positionner le joueur au centre de la fenêtre
    int playerX = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
    //int playerX = 700;
    int playerY = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;

        //// Afficher l'arrière-plan
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Afficher les cases de la carte
        for (int i = 0; i < taille_map; i++) {
            for (int j = 0; j < taille_map; j++) {
                SDL_Rect caseRect = { j * 71, i * 71, 71, 71 }; // taille d'une case est 32x32 pixels
                if (carte.cases[i][j] == 0) {
                    //SDL_RenderCopy(renderer, grassTexture, NULL, &caseRect);
                } else {
                    SDL_RenderCopy(renderer, wallTexture, NULL, &caseRect);
                }
            }
        }

        // Afficher le joueur
        SDL_Rect playerRect = { playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyTexture(playerTexture);
    SDL_FreeSurface(playerSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
