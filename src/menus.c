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
	char catalogName[2048];
	char outputFile[2048];

	putchar('\n');
	puts("----------------------------------------------------------");
	puts("                 HTML Catalog List Printing               ");
	puts("----------------------------------------------------------");
	printf("Type the name of the Catalog you want to print: ");

	INPUT_CLEAR;
	scanf("%[^\n]", catalogName);

	printf("Type the name of the output File: ");
	INPUT_CLEAR;
	scanf("%[^\n]", outputFile);
	
	generateList(catalogName, outputFile);
	
	return true;
}

bool queryMenu() {
	char  isbn[ISBN_SIZE + 1];
	char  catalogName[256];
	char  model[2049];
	char  OutputFile[2049];
	char * tmp_model;
	int c = 1;

	putchar('\n');
	puts("----------------------------------------------------------");
	puts("                          Query                           ");
	puts("----------------------------------------------------------");

	printf("Type the name of the Catalog you want to find a book: ");
	INPUT_CLEAR;

	while (! scanf("%[^\n]", catalogName) ) {
		printf("Invalid entry\n");

		if (! tryAgainMenu() )
			return true;

		printf("Type the name of the Catalog you want to find a book: ");
		INPUT_CLEAR;
	}

	printf("Type the ISBN of a Book for info: ");
	INPUT_CLEAR;
	
	while (! scanf("%[^\n]", isbn) ) {
		printf("Invalid entry\n");
		
		if (! tryAgainMenu() )
			return true;

		printf("Type the ISBN of a Book for info: ");
		INPUT_CLEAR;
	}
	
	while(c) {
		printf("Enter the name of the model file (or nothing for 'default.html'): ");
		INPUT_CLEAR;
		
		if (! scanf("%[^\n]", model) )
			strcpy(model, "default.html");
				
		if ( validateFile(model) == FILE_EXISTS )
			break;

		tmp_model = pathCat(MODEL_PATH, model);

		if( validateFile(tmp_model) != FILE_EXISTS ) {
			fprintf(stderr, "Model '%s', doesn't exist\n", tmp_model);
			free(tmp_model);
			c = tryAgainMenu();
		} else {
			strncpy(model, tmp_model, 2049);
			free(tmp_model);
			break;
		}
	}

	if(!c) return false;

	printf("Type the name for output HTML file: ");
	INPUT_CLEAR;
	
	while (! scanf("%[^\n]", OutputFile) ) {
		printf("Invalid entry\n");
		
		if (! tryAgainMenu() )
			return true;

		printf("Type the name of the output file: ");
		INPUT_CLEAR;
	}
	

	if( !(strlen(isbn) == ISBN_SIZE && validateISBN(isbn)) ) {
		fprintf(stderr, "Tried to set invalid ISBN: %s\n", isbn);
		return false;
	}

	query(catalogName, isbn, OutputFile, model);

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
