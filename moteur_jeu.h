#define TAILLE_MAP 7
int taille_map = TAILLE_MAP;

typedef struct map {
    int taille_map;
    int cases[TAILLE_MAP][TAILLE_MAP];
} map_t;

void affichage_carte(map_t carte);

void map_avec_fichier (char* nom_fichier, map_t* map);