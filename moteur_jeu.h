#define TAILLE_MAP 8
int taille_map = TAILLE_MAP;

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

void affichage_carte(map_t carte);

void map_avec_fichier (char* nom_fichier, map_t* map);

void init_joueur(int nb_joueurs, char* nom_fichier);