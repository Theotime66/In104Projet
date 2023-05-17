#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "gestionbombe.h"

#define TAILLE_MAP 14

const int WINDOW_WIDTH = 994;
const int WINDOW_HEIGHT = 994;
const int PLAYER_WIDTH = 71;
const int PLAYER_HEIGHT = 71;



//enum touches_autorisees {SDLK_q, SDLK_s, SDLK_d, SDLK_z, SDLK_l, SDLK_CAPSLOCK, SDLK_LSHIFT, SDLK_SEMICOLON, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN};
const int CHAR_AUTORISES[] = {SDLK_q, SDLK_s, SDLK_d, SDLK_z, SDLK_l, SDLK_CAPSLOCK, SDLK_LSHIFT, SDLK_SEMICOLON, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN};



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
    
    // Allouer dynamiquement le tableau de joueurs
    joueur_t* joueur = malloc(nb_joueurs * sizeof(joueur_t));
    
    // Vérifier si l'allocation a réussi
    if (joueur == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(1);
    }
        
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


pos_ij_t transformation_xy_ij (pos_xy_t position_xy){
    //Fonction permettant de transformer les coordonnées x y (sur la carte) en coordonnées i j (dans la matrice)
    int x = position_xy.x;
    int y = position_xy.y;

    int i = floor(x/71);
    int j = floor(y/71);

    pos_ij_t position_ij = {i, j};
    return position_ij;
}

pos_xy_t transformation_ij_xy (pos_ij_t position_ij){
    //Fonction permettant de transformer les coordonnées i j (dans la matrice) en coordonnées x y (sur la carte)
    //Remarque : cette fonction retourne le centre de la case
    int i = position_ij.i;
    int j = position_ij.j;

    int x = i*71 + 36;
    int y = j*71 + 36;

    pos_xy_t position_xy = {x, y};
    return position_xy;
}

int collision_J1_fleches (joueur_t joueur, map_t map, int touche_pressee){
    //Fonction permettant de géner les collisions dans le jeu.
    //Fonctionnement : si la case dans la direction du déplacement est un 0 : PAS DE PROBLEME : ON RETOURNE 0
    //Fonctionnement : si la case dans la direction du déplacement est autre chose que 0 : PROBLEME (COLLISION) : ON RETOURNE 1. Le déplacement est annulé

    int x_joueur = joueur.position_joueur.x;
    int y_joueur = joueur.position_joueur.y;
    pos_xy_t positionXY= {x_joueur, y_joueur};

    pos_ij_t positionIJ = transformation_xy_ij(positionXY);
    int i_joueur = positionIJ.i;
    int j_joueur = positionIJ.j;

    //Test de collision
    //Déplacement vers le haut
    if (touche_pressee == SDL_SCANCODE_UP){
        if(map.cases[i_joueur][j_joueur - 1] == 0){
            return 0;
        }
        else{
            //Affichage "Collision" dans le terminal s'il y a collision
            printf("Collision\n\n");
            return 1;
        }
    }
    //Déplacement vers le bas
    else if (touche_pressee == SDL_SCANCODE_DOWN){
        if(map.cases[i_joueur][j_joueur + 1] == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    //Déplacement vers la gauche
    else if (touche_pressee == SDL_SCANCODE_LEFT){
        if(map.cases[i_joueur - 1][j_joueur] == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    //Déplacement vers la droite
    else if (touche_pressee == SDL_SCANCODE_RIGHT){
        if(map.cases[i_joueur + 1][j_joueur] == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    //Touche appuyée ne correspond pas à un déplacement
    else {
        return 1;
    }

}

int collision_J2_lettres (joueur_t joueur, map_t map, int touche_pressee){
    //Fonction permettant de géner les collisions dans le jeu.
    //Fonctionnement : si la case dans la direction du déplacement est un 0 : PAS DE PROBLEME : ON RETOURNE 0
    //Fonctionnement : si la case dans la direction du déplacement est autre chose que 0 : PROBLEME (COLLISION) : ON RETOURNE 1. Le déplacement est annulé

    int x_joueur = joueur.position_joueur.x;
    int y_joueur = joueur.position_joueur.y;
    pos_xy_t positionXY= {x_joueur, y_joueur};

    pos_ij_t positionIJ = transformation_xy_ij(positionXY);
    int i_joueur = positionIJ.i;
    int j_joueur = positionIJ.j;

    //Test de collision
    //Déplacement vers le haut
    if (touche_pressee == SDL_SCANCODE_Z){
        if(map.cases[i_joueur][j_joueur - 1] == 0){
            return 0;
        }
        else{
            //Affichage "Collision" dans le terminal s'il y a collision
            //printf("Collision (test if)\n\n");
            return 1;
        }
    }
    //Déplacement vers le bas
    else if (touche_pressee == SDL_SCANCODE_S){
        if(map.cases[i_joueur][j_joueur + 1] == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    //Déplacement vers la gauche
    else if (touche_pressee == SDL_SCANCODE_Q){
        if(map.cases[i_joueur - 1][j_joueur] == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    //Déplacement vers la droite
    else if (touche_pressee == SDL_SCANCODE_D){
        if(map.cases[i_joueur + 1][j_joueur] == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    //Touche appuyée ne correspond pas à un déplacement
    else {
        return 1;
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

    /* JOUEURS */
    // Charger l'image du joueur 1 avec transparence
    SDL_Surface* playerSurface1 = SDL_LoadBMP("images/skin1.bmp");
    SDL_SetColorKey(playerSurface1, SDL_TRUE, SDL_MapRGB(playerSurface1->format, 0, 134.0, 53.0));
    SDL_Texture* playerTexture1 = SDL_CreateTextureFromSurface(renderer, playerSurface1);
    // Charger l'image du joueur 2 avec transparence
    SDL_Surface* playerSurface2 = SDL_LoadBMP("images/skin2.bmp");
    SDL_SetColorKey(playerSurface2, SDL_TRUE, SDL_MapRGB(playerSurface2->format, 0, 134.0, 53.0));
    SDL_Texture* playerTexture2 = SDL_CreateTextureFromSurface(renderer, playerSurface2);

    /* BOMBES */
    //Bombe 1
    SDL_Surface* bombeSurface1 = SDL_LoadBMP("images/bombe1.bmp");
    SDL_SetColorKey(bombeSurface1, SDL_TRUE, SDL_MapRGB(bombeSurface1->format, 255, 0, 255));
    SDL_Texture* bombeTexture1 = SDL_CreateTextureFromSurface(renderer, bombeSurface1);
    //Bombe 2
    SDL_Surface* bombeSurface2 = SDL_LoadBMP("images/bombe2.bmp");
    SDL_SetColorKey(bombeSurface2, SDL_TRUE, SDL_MapRGB(bombeSurface2->format, 255, 0, 255));
    SDL_Texture* bombeTexture2 = SDL_CreateTextureFromSurface(renderer, bombeSurface2);
    //Bombe 3
    SDL_Surface* bombeSurface3 = SDL_LoadBMP("images/bombe3.bmp");
    SDL_SetColorKey(bombeSurface3, SDL_TRUE, SDL_MapRGB(bombeSurface3->format, 255, 0, 255));
    SDL_Texture* bombeTexture3 = SDL_CreateTextureFromSurface(renderer, bombeSurface3);

    /* CASES */
    // Charger les images des cases
    SDL_Surface* wallSurface = SDL_LoadBMP("images/texture_mur.bmp");
    SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    SDL_Surface* wallUnbreakableSurface = SDL_LoadBMP("images/texture_mur_incassable.bmp");
    SDL_Texture* wallUnbreakableTexture = SDL_CreateTextureFromSurface(renderer, wallUnbreakableSurface);

    SDL_Surface* explosionSurface = SDL_LoadBMP("images/explosion.bmp");
    SDL_SetColorKey(explosionSurface, SDL_TRUE, SDL_MapRGB(explosionSurface->format, 255, 0, 255));
    SDL_Texture* explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);

    
    // Charger l'image de l'arrière-plan
    SDL_Surface* backgroundSurface = SDL_LoadBMP("images/background.bmp");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_Rect backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    /* POSITIONS */
    //Joueur 1
    int player1X = 9*71 ;
    int player1Y = 9*71 ;

    pos_xy_t positionJ1 = {player1X, player1Y};
    joueur_t joueur1 = {
        32,
        positionJ1,
        1,
        3,
        3,
    };

    //Joueur 2
    int player2X = 1*71;
    int player2Y = 2*71;

    pos_xy_t positionJ2 = {player2X, player2Y};
    joueur_t joueur2 = {
        31,
        positionJ2,
        1,
        3,
        3,
    };


    // Boucle de jeu
    SDL_Event event;
    int cont;
    int quit = 0;
    int collision_true; //Variable qui vaut 1 si il y a collision entre le joueur et un mur
    while (quit == 0) {
        // Gérer les événements
        while (SDL_PollEvent(&event)) {
            SDL_PumpEvents(); // On demande à la SDL de mettre à jour les états sur les périphériques
            // Clavier
            {
                    const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                    if ( pKeyStates[SDL_SCANCODE_ESCAPE] )
                    {
                        printf("Quit\n");
                        quit = 1;
                    }
                    if (event.type == SDL_QUIT)
                    {   
                        printf("Quit\n");
                        quit = 1;
                    }
                    if ( pKeyStates[SDL_SCANCODE_Q] )
                    {
                        if(collision_J2_lettres (joueur2, carte, SDL_SCANCODE_Q) == 0)
                            player2X -= joueur2.vitesse;
                    }
                    if ( pKeyStates[SDL_SCANCODE_Z] )
                    {
                        if(collision_J2_lettres (joueur2, carte, SDL_SCANCODE_Z) == 0)
                            player2Y -= joueur2.vitesse;
                    }if ( pKeyStates[SDL_SCANCODE_S] )
                    {
                        if(collision_J2_lettres (joueur2, carte, SDL_SCANCODE_S) == 0)
                            player2Y += joueur2.vitesse;
                    }if ( pKeyStates[SDL_SCANCODE_D] )
                    {
                        if(collision_J2_lettres (joueur2, carte, SDL_SCANCODE_D) == 0)
                            player2X += joueur2.vitesse;
                    }if ( pKeyStates[SDL_SCANCODE_K] )
                    {
                        bomb_t* bombe;
                        poser_bombe(&joueur2, &carte, bombe);
                    }if ( pKeyStates[SDL_SCANCODE_UP] )
                    {
                        if(collision_J1_fleches (joueur1, carte, SDL_SCANCODE_UP) == 0)
                            player1Y -= 5;
                    }if ( pKeyStates[SDL_SCANCODE_DOWN] )
                    {
                        if(collision_J1_fleches (joueur1, carte, SDL_SCANCODE_DOWN) == 0)
                            player1Y += 5;
                    }if ( pKeyStates[SDL_SCANCODE_LEFT] )
                    {
                        if(collision_J1_fleches (joueur1, carte, SDL_SCANCODE_LEFT) == 0)
                            player1X -= 5;
                    }if ( pKeyStates[SDL_SCANCODE_RIGHT] )
                    {
                        if(collision_J1_fleches (joueur1, carte, SDL_SCANCODE_RIGHT) == 0)
                            player1X += 5;
                    }if ( pKeyStates[SDL_SCANCODE_SEMICOLON] )
                    {
                        bomb_t* bombe;
                        poser_bombe(&joueur1, &carte, bombe);
                    }
                    if ( pKeyStates[SDL_SCANCODE_LSHIFT] )
                    {
                        cont = 0;
                    }
                    if ( pKeyStates[SDL_SCANCODE_CAPSLOCK] )
                    {
                        cont = 0;
                    }
                }



            /*
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        //Joueur 1
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

                        
                        //Joueur 2
                        
                        case SDLK_z:
                            player2Y -= joueur2.vitesse;
                            break;
                        case SDLK_s:
                            player2Y += joueur2.vitesse;
                            break;
                        case SDLK_q:
                            player2X -= joueur2.vitesse;
                            break;
                        case SDLK_d:
                            player2X += joueur2.vitesse;
                            break;
                    }
                    break;
            }*/
        }

        positionJ2.x = player2X;
        positionJ2.y = player2Y;
        pos_ij_t positionIJ = transformation_xy_ij(positionJ2);
        printf("i=%d,j=%d\n",positionIJ.i,positionIJ.j);

        joueur2.position_joueur = positionJ2;
        int var = collision_J2_lettres (joueur2, carte, 'z');
        //printf("Affichage collision :%d numéro %d\n",var,k);
        if (var == 1){
            //printf("COLLISION\n");
            quit;
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
                else if (carte.cases[i][j] == 45){
                    SDL_RenderCopy(renderer, explosionTexture, NULL, &caseRect);
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