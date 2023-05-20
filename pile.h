#ifndef _pile_h_
#define _pile_h_

#include <stdlib.h>

typedef int variant; /* remplacer int par le type de donnees a empiler */

/*---------------- STRUCTURES DE PILE.C --------------------------------------------*/

/*
  Structure :
  Nom  :  section
  Fct  :  structure definissant une section d'elements de type "variant"
*/

typedef struct section
{
  variant * tab;     /* tableau representant la section proprement dite */
  struct section * suiv;  /* pointeur sur la section suivante */
  struct section * prec;  /* pointeur sur la section precedente */
} section_t;

/*
  Structure :
  Nom  :  pile
  Fct  :  structure definissant une pile d'elements de type "variant"
*/

typedef struct pile
{
  int n;             /* nombre d'elements presents dans la pile moins un */
  int pas;           /* nombre d'elements dans une section de pile */
  section_t * tete;  /* pointeur de tete de la pile */
  section_t * sect;  /* pointeur sur la dernière section ajoutée */
} pile_t;


/*---------------- FONCTIONS DE PILE.C ---------------------------------------------*/

/*
  Nom  :  vide
  Fct  :  retourne un booleen indiquant si la pile est vide
  Entree  :  (p) adresse de la pile
  Sortie  :  booleen indiquant que la pile est vide
*/

unsigned short int vide(pile_t * p);

/*
  Nom  :  init
  Fct  :  cree une nouvelle pile et retourne son adresse
  Entree  :  (maxi) nombre maximal d'elements de la pile
  Sortie  :  adresse de la nouvelle pile
*/

pile_t * init(int pas);

/*
  Nom  :  empile
  Fct  :  empile un nouvel element sur la pile
  Entree  :  (p) adresse de la pile
             (x) element a rajouter
*/

void empile(pile_t * p, variant x);

/*
  Nom  :  depile
  Fct  :  depile un element de la pile et renvoie un booleen indiquant que tout s'est bien passe
  Entree  :  (p) adresse de la pile
             (x) adresse de stockage de l'element depile
  Sortie  :  booleen indiquant que tout s'est bien passe
*/

unsigned short int depile(pile_t * p, variant * x);

/*
Nom  :  supprime
Fct  :  libère toute la mémoire occupée par une pile
Entree  :  (p) adresse de la pile
*/

void supprime(pile_t * p);

#endif
