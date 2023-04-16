#include <stdio.h>
#include <SDL2/SDL.h>

// Compilation : gcc -o jeu_video jeu_video.c -lSDL2

int main(int argc, char* args[]) {
    // Initialiser SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Créer la fenêtre
    SDL_Window* window = SDL_CreateWindow("Jeu vidéo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    // Créer le rendu de la fenêtre
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger l'image du joueur
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    // Positionner le joueur au centre de la fenêtre
    int playerX = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
    int playerY = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;

    // Boucle de jeu
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        // Gérer les événements
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            playerX -= 5;
                            break;
                        case SDLK_RIGHT:
                            playerX += 5;
                            break;
                        case SDLK_UP:
                            playerY -= 5;
                            break;
                        case SDLK_DOWN:
                            playerY += 5;
                            break;
                    }
                    break;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Afficher le joueur
        SDL_Rect playerRect = { playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyTexture(playerTexture);
    SDL_FreeSurface(playerSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
