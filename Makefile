CC=gcc
CFLAGS=-Wall -Wfatal-error
INC=-I include/
SRC=src/
EXEC=main
all: $(EXEC)
main: $(SRC)main.c $(SRC)article.o
	$(CC) $(INC) -o $(SRC)$@ $^ $(CFLAGS)

$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -o $@ -c $< $(CFLAGS)
clean:
	rm -rf $(SRC)*.o