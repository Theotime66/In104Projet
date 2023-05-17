#define TAILLE_MAP 14

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

typedef struct bombe{
    pos_ij_t pos_bombe;
    int temps_explosion;
    int rayon;
} bomb_t; 

void affichage_carte(map_t carte);

void map_avec_fichier (char* nom_fichier, map_t* map);

joueur_t* init_joueur(int nb_joueurs, char* nom_fichier);

void affichage_jeu(map_t carte);

pos_ij_t transformation_xy_ij (pos_xy_t position_xy, int offsetX, int offsetY);

pos_xy_t transformation_ij_xy (pos_ij_t position_ij);

int collision_J1_fleches (joueur_t joueur, map_t map, int touche_pressee);

int collision_J2_lettres (joueur_t joueur, map_t map, int touche_pressee);