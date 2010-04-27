#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "catalog.h"
#include "menus.h"
#include "tools.h"

int mainMenu() {

	char aux;

	system("clear");
	puts("==========================================================");
	puts("                     BOOK CATALOG                         ");
	puts("==========================================================");
	putchar('\n');
	puts("         First of all, pick up an option:                 ");
	putchar('\n');
	puts("+--------------------------------------------------------+");
	puts("| Option | What it does.                                 |");
	puts("+========+===============================================+");
	puts("|   'c'  | Creates a new Catalog file, which contains    |");
	puts("|        | information about books.                      |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'h'  | Gives you a hand with some manuals when       |");
	puts("|        | necessary.                                    |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'a'  | Asks you some questions in order to add a new |");
	puts("|        | book to a specific Catalog.                   |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'q'  | Searches for a book, by ISBN, displaying HTML |");
	puts("|        | info. (Present version searches only by ISBN).|");
	puts("+--------+-----------------------------------------------+");
	puts("|   'l'  | Gives you an HTML organized list of books     |");
	puts("|        | that belong to a Catalog.                     |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'm'  | Shows this menu.                              |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'x'  | Exits Program.                                |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	putchar('\n');
	printf("Type your option: ");

	INPUT_CLEAR;
	scanf("%c", &aux);

	return (int)(toupper(aux));
}

int retypeOptionMenu() {
	printf("Invalid option. Try again, or type h for help: ");
	INPUT_CLEAR;

	return (int)toupper(getchar());
}

bool tryAgainMenu() {
	char c;
	printf("Would you like to try again? (y/n) ");

	while(1) {
		INPUT_CLEAR;
		c = getchar();

		switch (toupper(c)){
			case 'Y':
				return true;
			case 'N':
				return false;
		}
	}
}

bool listprintMenu() {
	char catalogName[200];

	putchar('\n');
	puts("----------------------------------------------------------");
	puts("                 HTML Catalog List Printing               ");
	puts("----------------------------------------------------------");
	printf("Type the name of the Catalog you want to print: ");

	INPUT_CLEAR;
	scanf("%[^\n]", catalogName);
	
	generateList(catalogName);
	
	return true;
}

bool queryMenu() {
	char  isbn[ISBN_SIZE + 1];
	char  catalogName[256];

	putchar('\n');
	puts("----------------------------------------------------------");
	puts("                          Query                           ");
	puts("----------------------------------------------------------");

	printf("Type the name of the Catalog you want to find a book: ");
	INPUT_CLEAR;
	memset(catalogName, 'X', 256);
	scanf("%[^\n]", catalogName);

	printf("Type the ISBN of a Book for info: ");
	INPUT_CLEAR;
	memset(isbn, 32, ISBN_SIZE + 1);
	scanf("%[^\n]", isbn);

	if( !(strlen(isbn) == ISBN_SIZE && validateISBN(isbn)) ) {
		fprintf(stderr, "Tried to set invalid ISBN: %s\n", isbn);
		return false;
	}

	query(catalogName, isbn);

	return true;
}

bool addBookMenu() {
	char auxstr[MAXSTRING_SIZE];
	char catalogName[MAXSTRING_SIZE];
	Book* newBook;
	FILE* Catalog;

	/*Array of structs containing questions and functions to be called later in next for()*/
	struct {
		char * info;
		bool (*func)(Book*, char*);
	}BookInfo[] = {
		{"Type the Title of the book:", setTitle},
		{"Type the Author of the book:", setAuthor},
		{"Type the Main Characters of the book (comma-separated list):", setCharacter},
		{"Type the Summary of the book:", setSummary},
		{"Type the Subject of the book:", setSubject},
		{"Type the ISBN of the book:", setISBN},
		{"Type the book's Image Filename:", setImgFile},
		{"Type the Year of publication:", setYear}
		};

		putchar('\n');
		puts("----------------------------------------------------------");
		puts("                          Add Book                        ");
		puts("----------------------------------------------------------");

		/*Book and Catalog allocation*/
		do{
			printf("Type the name of the file where you want to append a book: ");
			INPUT_CLEAR;
			scanf("%[^\n]", catalogName);
			if((Catalog = openFile(catalogName, "a+")))
				break;
		}while(tryAgainMenu());

		/*Checks Book and Catalog allocation*/
		if(!Catalog)
			return false;

		if(!(newBook = createBook())){
			fclose(Catalog);
			return false;
		}

		/*Start calling functions from bookinfo*/
		for(int i=0 ; i<8; i++){
			printf("%s ", BookInfo[i].info);
			INPUT_CLEAR;

			if (! scanf("%[^\n]", auxstr) ) {
   printf("Can't work with empty strings.\n");

   if ( tryAgainMenu() ) {
	   i--;
	   continue;
   } else {
	   return false;
   }
   }

   if( !(BookInfo[i].func(newBook, auxstr)) ){
	   if(tryAgainMenu())
		   i--;
	   else
		   return false;
   }
		}

		/*Writes to catalog using macro included in books.h*/
		writeBook(newBook, Catalog);

		/*Prints success*/
		puts("----------------------------------------------------------");
		puts("               Book added successfully!                   ");
		puts("----------------------------------------------------------");

		/*Closes file and frees memory*/
		fclose(Catalog);
		freeBook(newBook);

		return true;
		}

		bool createCatalogMenu() {
			char auxstr[200];
			FILE* Catalog;

			putchar('\n');
			puts("----------------------------------------------------------");
			puts("                      Create New Catalog                  ");
			puts("----------------------------------------------------------");
			printf("Type the name of the new catalog: ");

			INPUT_CLEAR;
			scanf("%[^\n]", auxstr);

			if( (Catalog = createFile(auxstr, "a+")) ){
				printf("'%s' succesfully created", auxstr);
				fclose(Catalog);
				return true;
			}else{
				fprintf(stderr, "New Catalog could not be created");
				return false;
			}

		}

		void helpMenu() {
			printHelp();

			return;
		}

		int continueMenu() {
			printf("\nWould you like to continue (y/n) ? ");

			INPUT_CLEAR;
			switch (toupper(getchar())) {
				case 'Y' : return 'M';
				case 'N' : return 'X';
				default: return 'H';
			}
		}
