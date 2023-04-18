#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


#define TAILLE_MAP 14
const int WINDOW_WIDTH = 994;
const int WINDOW_HEIGHT = 994;
const int PLAYER_WIDTH = 71;
const int PLAYER_HEIGHT = 71;


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
    //Permet d'acquérir : taille map, nombre de joueur, position de chaque joueur, carte (cases)

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
    int skin;
    for (int k = 0; k<nombre_joueurs; k++){
        fscanf(file, "%d", &x); 
        fscanf(file, "%d", &y);
        fscanf(file, "%d", &skin);

        joueur[k].position_joueur.x = x;
        joueur[k].position_joueur.y = y;
        joueur[k].skin = skin;
    }

    //Initialisation de la carte
    for (int i = 0; i < taille_map; i++) {
        for (int j = 0; j < taille_map; j++) {
            //printf("i=%d,j=%d\n",i,j);
            fscanf(file, "%d", &map->cases[i][j]);
        }
    }

    //Initialisation du joueur sur la carte
    for (int k = 0; k<nombre_joueurs; k++){
        x = joueur[k].position_joueur.x;
        y = joueur[k].position_joueur.y;
        skin = joueur[k].skin;

        map->cases[x][y] = skin;
    }


    fclose(file);
    map->taille_map = taille_map;
}


joueur_t init_joueur(int nb_joueurs, char* nom_fichier){
    
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
    
    if(nb_joueurs != nb_j){
        printf("Pas le bon nombre de joueurs.\n");
        exit(1);
    }
    
    for(int i=0; i< nb_joueurs; ++i){
        
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
    return(joueur);
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

    // Charger l'image du joueur
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    // Charger les images des cases
    SDL_Surface* grassSurface = SDL_LoadBMP("texture_herbe.bmp");
    SDL_Texture* grassTexture = SDL_CreateTextureFromSurface(renderer, grassSurface);
    SDL_Surface* wallSurface = SDL_LoadBMP("texture_mur.bmp");
    SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);

    
    // Charger l'image de l'arrière-plan
    SDL_Surface* backgroundSurface = SDL_LoadBMP("background.bmp");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_Rect backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    // Positionner le joueur au centre de la fenêtre
    //int playerX = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
    int playerX = 700;
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

        //// Afficher l'arrière-plan
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Afficher les cases de la carte
        for (int i = 0; i < taille_map; i++) {
            for (int j = 0; j < taille_map; j++) {
                SDL_Rect caseRect = { j * 71, i * 71, 71, 71 }; // taille d'une case est 32x32 pixels
                if (carte.cases[i][j] == 0) {
                    SDL_RenderCopy(renderer, grassTexture, NULL, &caseRect);
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