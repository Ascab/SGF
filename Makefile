CC=gcc
CFLAGS= -Wall

all: sgf

sgf: Sources/cmd.o Sources/io.o Sources/main.o Sources/my_string.o Sources/manip_fichier.o Sources/manip_disk.o
	$(CC) -o sgf $^ $(CFLAGS)
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
clean: 
	rm -f Sources/*.o
mrproper: clean
	rm -f sgf
	
