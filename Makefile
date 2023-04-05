CC=gcc
CFLAGS=-I.

mon_programme: main.o moteur_jeu.o
	$(CC) -o mon_programme main.o moteur_jeu.o

main.o: main.c moteur_jeu.h
	$(CC) -c main.c

moteur_jeu.o: moteur_jeu.c moteur_jeu.h
	$(CC) -c moteur_jeu.c

clean:
	rm -f *.o mon_programme
