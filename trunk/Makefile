CFLAGS=-Iinclude -Wall -g -std=c99

catalog: main.o tools.o books.o catalog.o interfaces.o menus.o index.o
	gcc $(CFLAGS) -O2 -o catalog main.o tools.o books.o catalog.o interfaces.o menus.o index.o

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

index.o:
	gcc $(CFLAGS) -c src/index.c

clean:
	rm -f *.o
