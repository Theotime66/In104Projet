#define TAILLE_MAP 14
int taille_map = TAILLE_MAP;


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

void affichage_carte(map_t carte);

void map_avec_fichier (char* nom_fichier, map_t* map);

void init_joueur(int nb_joueurs, char* nom_fichier);

void affichage_jeu ();

pos_ij_t transformation_xy_ij (pos_xy_t position_xy);

pos_xy_t transformation_ij_xy (pos_ij_t position_ij);

int collision (joueur_t joueur, map_t map, int touche_pressee);