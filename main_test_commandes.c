#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "generic_queue.h"
#include "commande.h"

int main(){
    int **a;
    generic_queue_t fifo; 
    generic_queue_init(&fifo);
    printf("Entrée dans la boucle infinie :\n");
    while (1){
        printf("avant commande(0)\n");
        fifo = commande(0);
        printf("commande(0) ok\n");
        generic_queue_dequeue(&fifo, a);

        printf("a = %d\n",**a);
    }
    printf("Sortie dans la boucle infinie :\n");
    return 0;
}