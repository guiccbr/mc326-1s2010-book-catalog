CFLAGS=-Iinclude -Wall -std=c99

catalog: main.o tools.o books.o catalog.o interfaces.o menus.o
	gcc $(CFLAGS) -O2 -o catalog main.o tools.o books.o catalog.o interfaces.o menus.o

main.o:
	gcc $(CFLAGS) -c src/main.c

tools.o:
	gcc $(CFLAGS) -c src/tools.c

books.o:
	gcc $(CFLAGS) -c src/books.c

catalog.o:
	gcc $(CFLAGS) -c src/catalog.c

interfaces.o:
	gcc $(CFLAGS) -c src/interfaces.c

menus.o:
	gcc $(CFLAGS) -c src/menus.c

clean:
	rm -f *.o
