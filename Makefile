CC=gcc
CFLAGS=-Wall -Wfatal-errors

mon_programme: main.o moteur_jeu.o gestionbombe.o
	$(CC) $(CFLAGS) -o mon_programme main.o moteur_jeu.o gestionbombe.o -lSDL2 -lm

main.o: main.c moteur_jeu.h
	$(CC) -c main.c

moteur_jeu.o: moteur_jeu.c moteur_jeu.h
	$(CC) -c moteur_jeu.c

gestionbombe.o: gestionbombe.c gestionbombe.h
	$(CC) $(CFLAGS) -c gestionbombe.c

clean:
	rm -f *.o mon_programme
