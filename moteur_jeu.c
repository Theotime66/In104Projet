#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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


joueur_t* init_joueur(int nb_joueurs, char* nom_fichier){
    
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

        pos_xy_t position_init[i];
        fscanf(file,"%d",&position_init[i].x);
        fscanf(file,"%d",&position_init[i].y);


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

    /* JOUEURS */
    // Charger l'image du joueur 1 avec transparence
    SDL_Surface* playerSurface1 = SDL_LoadBMP("player.bmp");
    SDL_SetColorKey(playerSurface1, SDL_TRUE, SDL_MapRGB(playerSurface1->format, 0, 134.0, 53.0));
    SDL_Texture* playerTexture1 = SDL_CreateTextureFromSurface(renderer, playerSurface1);
    // Charger l'image du joueur 2 avec transparence
    SDL_Surface* playerSurface2 = SDL_LoadBMP("skin2.bmp");
    SDL_SetColorKey(playerSurface2, SDL_TRUE, SDL_MapRGB(playerSurface2->format, 0, 134.0, 53.0));
    SDL_Texture* playerTexture2 = SDL_CreateTextureFromSurface(renderer, playerSurface2);

    /* BOMBES */
    //Bombe 1
    SDL_Surface* bombeSurface1 = SDL_LoadBMP("bombe1.bmp");
    SDL_SetColorKey(bombeSurface1, SDL_TRUE, SDL_MapRGB(bombeSurface1->format, 255, 0, 255));
    SDL_Texture* bombeTexture1 = SDL_CreateTextureFromSurface(renderer, bombeSurface1);
    //Bombe 2
    SDL_Surface* bombeSurface2 = SDL_LoadBMP("bombe2.bmp");
    SDL_SetColorKey(bombeSurface2, SDL_TRUE, SDL_MapRGB(bombeSurface2->format, 255, 0, 255));
    SDL_Texture* bombeTexture2 = SDL_CreateTextureFromSurface(renderer, bombeSurface2);
    //Bombe 3
    SDL_Surface* bombeSurface3 = SDL_LoadBMP("bombe3.bmp");
    SDL_SetColorKey(bombeSurface3, SDL_TRUE, SDL_MapRGB(bombeSurface3->format, 255, 0, 255));
    SDL_Texture* bombeTexture3 = SDL_CreateTextureFromSurface(renderer, bombeSurface3);

    /* CASES */
    // Charger les images des cases
    //SDL_Surface* grassSurface = SDL_LoadBMP("texture_herbe.bmp");
    //SDL_Texture* grassTexture = SDL_CreateTextureFromSurface(renderer, grassSurface);
    SDL_Surface* wallSurface = SDL_LoadBMP("texture_mur.bmp");
    SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    SDL_Surface* wallUnbreakableSurface = SDL_LoadBMP("texture_mur_incassable.bmp");
    SDL_Texture* wallUnbreakableTexture = SDL_CreateTextureFromSurface(renderer, wallUnbreakableSurface);

    
    // Charger l'image de l'arrière-plan
    SDL_Surface* backgroundSurface = SDL_LoadBMP("background.bmp");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_Rect backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    /* POSITIONS */
    // Positionner le joueur au centre de la fenêtre
<<<<<<< Updated upstream
    int player1X = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
    //int playerX = 700;
    int player1Y = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;
    //Joueur 2
    int player2X = 200;
    int player2Y = 200;

=======
    int playerX = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
    //int playerX = 700;
    int playerY = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;
>>>>>>> Stashed changes

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
                            player1X -= 5;
                            break;
                        case SDLK_RIGHT:
                            player1X += 5;
                            break;
                        case SDLK_UP:
                            player1Y -= 5;
                            break;
                        case SDLK_DOWN:
                            player1Y += 5;
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
                    //SDL_RenderCopy(renderer, grassTexture, NULL, &caseRect);
                //Affichage des murs
                } else if (carte.cases[i][j] == 1) {
                    SDL_RenderCopy(renderer, wallTexture, NULL, &caseRect);
                }
                else if (carte.cases[i][j] == 11) {
                    SDL_RenderCopy(renderer, wallUnbreakableTexture, NULL, &caseRect);
                }
                else if (carte.cases[i][j] == 41){
                    SDL_RenderCopy(renderer, bombeTexture1, NULL, &caseRect);
                }
                else if (carte.cases[i][j] == 42){
                    SDL_RenderCopy(renderer, bombeTexture2, NULL, &caseRect);
                }
                else if (carte.cases[i][j] == 43){
                    SDL_RenderCopy(renderer, bombeTexture3, NULL, &caseRect);
                }
            }
        }

        // Afficher le joueur
        SDL_Rect playerRect1 = { player1X, player1Y, PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_RenderCopy(renderer, playerTexture1, NULL, &playerRect1);
        SDL_Rect playerRect2 = { player2X, player2Y, PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_RenderCopy(renderer, playerTexture2, NULL, &playerRect2);


        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyTexture(playerTexture1);
    SDL_DestroyTexture(playerTexture2);
    SDL_FreeSurface(playerSurface1);
    SDL_FreeSurface(playerSurface2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
pos_ij_t transformation_xy_ij (pos_xy_t position_xy){
    //Fonction permettant de transformer les coordonnées x y (sur la carte) en coordonnées i j (dans la matrice)
    float x = position_xy.x;
    float y = position_xy.y;

    int i = floor(x);
    int j = floor(y);

    pos_ij_t position_ij = {i, j};
    return position_ij;
}

pos_xy_t transformation_ij_xy (pos_ij_t position_ij){
    //Fonction permettant de transformer les coordonnées i j (dans la matrice) en coordonnées x y (sur la carte)
    //Remarque : cette fonction retourne le centre de la case
    int i = position_ij.i;
    int j = position_ij.j;

    float x = i*71 + 36;
    float y = j*71 + 36;

    pos_xy_t position_xy = {x, y};
    return position_xy;
}

typedef struct bombe{
    pos_xy_t pos_bombe;
    int timer;
} bomb_t;

void est_mort(int nb_joueurs, joueur_t* joueurs, generic_queue_t** cmd /* la fifo*/){
    for(int i=0; i<nb_joueurs; i++){
        if(joueurs[i].nb_vies==0){
            generic_queue_enqueue(*cmd,joueurs[i].skin 'dead');/*mettre dans la fifo cmdgraphique que le joueur i (définie par son skin) est mort pour faire l'animation */
            for(int k=i; k<nb_joueurs-1 ;k++){
                joueurs[k]=joueurs[k+1]; // on décale tous les joueurs à partir i ème, cela permet d'éliminer le joueur qui n'a plus de vie
            }
            nb_joueurs--; // on abaisse le nombre de joueurs 
        }
    }
}

int a_gagne(int nb_joueurs, joueur_t* joueurs, generic_queue_t** cmd /*peut être la fifo*/ ){
    if(nb_joueurs>1){
        return 0;
    }
    else{
        generic_queue_enqueue(*cmd,joueurs[1].skin'win');// commande dans la fifo pour afficher peut ^tre pas la bonne expression dans la file cmdgraphique
        return 1;
    }
}

void poser_bombe(map_t carte, generic_queue_t** cmd){

    if(/* case de la bombe est vide et le joueur a assez de bombe*/)
}