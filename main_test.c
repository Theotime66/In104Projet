#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0){
        printf("error%s\n",SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow ("GAME",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000,1000,0);
    while (1)
    {
        ;
    }
    return 0;
    
}