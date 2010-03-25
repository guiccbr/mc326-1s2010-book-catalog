CFLAGS=-Iinclude -Wall -std=c99

catalog: main.o books.o catalog.o tools.o noninteractive.o interactive.o
	gcc $(CFLAGS) -o catalog main.o books.o catalog.o tools.o noninteractive.o interactive.o


main.o:
	gcc $(CFLAGS) -c src/main.c

books.o:
	gcc $(CFLAGS) -c src/books.c

catalog.o:
	gcc $(CFLAGS) -c src/catalog.c

tools.o:
	gcc $(CFLAGS) -c src/tools.c

noninteractive.o:
	gcc $(CFLAGS) -c src/noninteractive.c

interactive.o:
	gcc $(CFLAGS) -c src/interactive.c

clean:
	rm -f *.o catalog	
