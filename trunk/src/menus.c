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
	puts("|   'b'  | Books tools. Here you can manage a catalog    |");
	puts("|        | adding and/or removing books.                 |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'h'  | Gives you a hand with some manuals when       |");
	puts("|        | necessary.                                    |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'q'  | Searches for a book, or a set of books,       |");
	puts("|        | displaying an HTML info.                      |");
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

	while(1) {
		printf("Would you like to try again? (y/n) ");
		INPUT_CLEAR;
		c = getchar();

		switch (toupper(c)){
			case 'Y':
				return true;
			case 'N':
				return false;
			default:
				printf("Invalid option. ");
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

	/* Reads Catalog Name*/
	while (1) {
		printf("Type the name of the Catalog you want to print: ");
		INPUT_CLEAR;

		if( !scanf("%[^\n]", catalogName) ) {
			printf("Invalid Entry\n");
			if (!tryAgainMenu())
				return true;
		}else if( validateFile(catalogName) != FILE_EXISTS ) {
			printf("File '%s' does not exist\n", catalogName);
			if (!tryAgainMenu())
				return true;
		}
		else
			break;
	}
	
	/*Reads Output file Name*/
	while (1) {
		printf("Type the name for output HTML file: ");
		INPUT_CLEAR;

		if( !scanf("%[^\n]", outputFile) ) {
			printf("Invalid Entry\n");
			if (!tryAgainMenu())
				return true;
		}
		else
			break;
	}
	
	return generateList(catalogName, NULL, outputFile);
}

int queryMenu() {

	char aux;

	system("clear");
	puts("==========================================================");
	puts("                       Query words                        ");
	puts("==========================================================");
	putchar('\n');
	puts("                     Pick up an option:                   ");
	putchar('\n');
	puts("+--------------------------------------------------------+");
	puts("| Option | What it does.                                 |");
	puts("+========+===============================================+");
	puts("|   'A'  | Search by Author.                             |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'T'  |  Search by Title.                             |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'S'  | Search by Subject.                            |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'Y'  | Search by Year.                               |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'I'  | Search by ISBN.                               |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'C'  | Combined Search: Search for a set of books.   |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'm'  | Go back to main menu.                         |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	putchar('\n');

	do {
		printf("Type your option: ");
		INPUT_CLEAR;
		scanf("%c", &aux);
		switch(toupper(aux)) {
			case 'A':
				if (searchMenu(AUTHOR))
					return continueMenu();
			case 'T':
				searchMenu(TITLE);
				return continueMenu();
			case 'I':
				searchMenu(ISBN);
				return continueMenu();
			case 'S':
				searchMenu(SUBJECT);
				return continueMenu();
			case 'Y':
				searchMenu(YEAR);
				return continueMenu();
			case 'C':
				searchMenu(-1);
				return continueMenu();			
			case 'M':
				return 'M';
			default:
				printf("Invalid Option %c\n", aux);
		}
	}while(tryAgainMenu());
	
	return continueMenu();
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
			if((Catalog = openFile(catalogName, "r+")))
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
					/*Remember to Remove created File Here*/
					return false;
				}
			}

			if( !(BookInfo[i].func(newBook, auxstr)) ){
				if(tryAgainMenu())
					i--;
				else
					/*Remember to Remove created File Here*/
					return false;
			}
		}

		/*Adds Book to catalog*/
		if(!addBook(Catalog, newBook)) {
			fprintf(stderr, "Internal Error. Book wasn't successfully written to catalog.\n");
			fclose(Catalog); freeBook(newBook); return false;
		}
		
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
				fprintf(stderr, "New Catalog could not be created\n");
				return false;
			}

		}

void helpMenu() {
			printHelp();

			return;
		}

int continueMenu() {
			printf("\n");
			while(1) {
				printf("Would you like to continue (y/n) ? ");
				INPUT_CLEAR;
				switch (toupper(getchar())) {
					case 'Y' : return 'M';
					case 'N' : return 'X';
					default: printf("Invalid option."); 
				}
			}
}	
		
int bookToolsMenu() {
		char aux;

	system("clear");
	puts("==========================================================");
	puts("                        BOOK TOOLS                        ");
	puts("==========================================================");
	putchar('\n');
	puts("                     Pick up an option:                   ");
	putchar('\n');
	puts("+--------------------------------------------------------+");
	puts("| Option | What it does.                                 |");
	puts("+========+===============================================+");
	puts("|   'a'  | Asks you some questions in order to add a new |");
	puts("|        | book to a specific Catalog.                   |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'r'  | Removes a single book, or a set of books,     |");
	puts("|        | from a specific Catalog.                      |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'm'  | Go back to main menu                          |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	putchar('\n');

	do {
		printf("Type your option: ");
		INPUT_CLEAR;
		scanf("%c", &aux);
		switch(toupper(aux)) {
			case 'A':
				addBookMenu();
				return continueMenu();
			case 'R':
				rmBookMenu();
				return continueMenu();	
			case 'M':
				return 'M';
			default:
				printf("Invalid Option %c\n", aux);
		}
	}while(tryAgainMenu());
	
	return 'm';
}

bool rmBookMenu() {

	/*Aux*/
	enum IndexType search_by;
	int rrns[10000];
	char aux;
	char catalogName[2049];
	int found_no = 0;
	
	/*Search Keys*/
	char isbn[ISBN_SIZE + 1];
	char title[TITLE_SIZE + 1];
	char subject[SUBJECT_SIZE + 1];
	char year[YEAR_SIZE + 1];
	char author[AUTHOR_SIZE + 1];
	
	system("clear");
	puts("==========================================================");
	puts("                       Query words                        ");
	puts("==========================================================");
	putchar('\n');
	puts("                     Pick up an option:                   ");
	putchar('\n');
	puts("+--------------------------------------------------------+");
	puts("| Option | What it does.                                 |");
	puts("+========+===============================================+");
	puts("|   'i'  | Removes a Book directly by its ISBN.          |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'c'  | Removes a set of Books performing a           |");
	puts("|        | combined search.                              |");
	puts("+--------+-----------------------------------------------+");
	puts("|   'm'  | Go back to main menu.                         |");
	puts("|        |                                               |");
	puts("+--------+-----------------------------------------------+");
	putchar('\n');
	
	while(1) {
		printf("Type your option: ");
		INPUT_CLEAR;
		scanf("%c", &aux);
		if (toupper(aux) == 'I') {
				search_by = ISBN;
				break;
		}
		else if (toupper(aux) == 'C') {
			search_by = -1;
			break;
		}
		else
			if(tryAgainMenu())
				continue;
			else return false;
	}
	
	/* Reads Catalog Name*/
	while (1) {
		printf("Type the name of the Catalog for query: ");
		INPUT_CLEAR;

		if( !scanf("%[^\n]", catalogName) ) {
			printf("Invalid Entry\n");
			if (!tryAgainMenu())
				return true;
		}else if( validateFile(catalogName) != FILE_EXISTS ) {
			printf("File '%s' does not exist\n", catalogName);
			if (!tryAgainMenu())
				return true;
		}
		else
			break;
	}
	                    
	if(combinedSearchMenu(isbn, title, author, year, subject, search_by)) {
		found_no = queryKeyWords(catalogName, check_str(isbn), check_str(title), check_str(year), check_str(author), check_str(subject), rrns);
		if(found_no > 0)
			removeBooks(catalogName, rrns);
		else if (found_no == 0) {
			printf("No book matches search\n");
			return true;
		}
		else return false;
	}
	else return false;
	
	return true;
}
bool combinedSearchMenu(char * isbn, char * title, char * author, char * year, char * subject, enum IndexType type) {
	char temp_str[2048];
	
	if(type == -1) {
		printf("\nType information for a combined search. Empty strings are not considered.\n");
	}
	
	/*Cleans input strings*/
	isbn[0] = title[0] = author[0] = year[0] = subject[0] = '\0';
	
	while (1) {
			/*Reads ISBN*/
			while (type == ISBN || type == -1) {
				printf("ISBN: ");
				INPUT_CLEAR;

				if( !scanf("%[^\n]", temp_str) ) {
					break;
				}
				else if( ( strlen(temp_str) != ISBN_SIZE ) || !validateISBN(temp_str) ) {
					printf("'%s' out of ISBN standards\n", temp_str);
					if (!tryAgainMenu())
						return false;
				}
				else {
					memcpy(isbn, temp_str, (ISBN_SIZE + 1)*sizeof(char));
					break;
				}
			}
			/*Reads Year*/
			while (type == YEAR || type == -1) {
				printf("Year: ");
				INPUT_CLEAR;

				if( !scanf("%[^\n]", temp_str) ) {
					break;
				}
				else if( ( strlen(temp_str) != YEAR_SIZE ) || !validateYear(temp_str) ) {
					printf("'%s' out of Year standards\n", temp_str);
					if (!tryAgainMenu())
					 return false;
				}
				else {
					memcpy(year, temp_str, (YEAR_SIZE + 1)*sizeof(char));
					break;
				}
			}
			/*Reads Title*/
			while (type == TITLE || type == -1) {
				printf("Title: ");
				INPUT_CLEAR;
				
				if( !scanf("%[^\n]", temp_str) ) {
					break;
				}
				else if( strlen(temp_str) > TITLE_SIZE ) {
					printf("'%s' is too long for a book title", temp_str);
					if (!tryAgainMenu())
						return false;
				}
				else {
					memcpy(title, temp_str, (TITLE_SIZE + 1)*sizeof(char));
					break;
				}
			}
			/*Reads Author*/
			while (type == AUTHOR || type == -1) {
				printf("Author: ");
				INPUT_CLEAR;
				
				if( !scanf("%[^\n]", temp_str) ) {
					break;
				}
				else if( strlen(temp_str) > AUTHOR_SIZE ) {
					printf("'%s' is too long for an author", temp_str);
					if (!tryAgainMenu())
						return false;
				}
				else {
					memcpy(author, temp_str, (AUTHOR_SIZE + 1)*sizeof(char));
					break;
				}
			}
			/*Reads Subject*/
			while (type == SUBJECT || type == -1) {
				printf("Subject: ");
				INPUT_CLEAR;
				
				if( !scanf("%[^\n]", temp_str) ) {
					break;
				}
				else if( strlen(temp_str) > SUBJECT_SIZE ) {
					printf("'%s' is too long for a subject word", temp_str);
					if (!tryAgainMenu())
						return false;
				}
				else {
					memcpy(subject, temp_str, (SUBJECT_SIZE + 1)*sizeof(char));
					break;
				}
			}
			/*Checks if all fields are empty - No key lists the whole catalog*/
			if(
				(isbn[0]=='\0') &&
				(title[0]=='\0') &&
				(author[0]=='\0') &&
				(subject[0]=='\0') &&
				(year[0]=='\0')) {
				printf("All keys are empty. This search results in a set that contains the whole catalog\n");
				if(continueMenu() == 'M') break;
				else continue;
			}
						
			/*Confirms search*/
			printf("\nYour search:\n");
			if(isbn[0]!='\0') printf("ISBN: %s\n", isbn);
			if(title[0]!='\0') printf("Title: %s\n", title);
			if(author[0]!='\0') printf("Author: %s\n", author);
			if(subject[0]!='\0') printf("Subject: %s\n", subject);
			if(year[0]!='\0') printf("Year: %s\n", year);
			if(yesOrNoMenu("\nAre you sure you want to search for these words?")) {
				printf("Searching...\n\n");
				break;
			}
			else if (tryAgainMenu())
				continue;
			else return false;
		}
		
		return true;
}
bool searchMenu(enum IndexType type) {	
	
	/*Definition. Returns Null if a string is iniciated by '\0' (Empty string)*/
	
	/*Auxliliar*/
	char  catalogName[2049];
	char  model[2049];
	char  outputFile[2049];
	char * tmp_model;
	int rrns[10000];
	int found_no = 0;
	
	/*Search Keys*/
	char isbn[ISBN_SIZE + 1];
	char title[TITLE_SIZE + 1];
	char subject[SUBJECT_SIZE + 1];
	char year[YEAR_SIZE + 1];
	char author[AUTHOR_SIZE + 1];
	
	/* Reads Catalog Name*/
	while (1) {
		printf("Type the name of the Catalog for query: ");
		INPUT_CLEAR;

		if( !scanf("%[^\n]", catalogName) ) {
			printf("Invalid Entry\n");
			if (!tryAgainMenu())
				return true;
		}else if( validateFile(catalogName) != FILE_EXISTS ) {
			printf("File '%s' does not exist\n", catalogName);
			if (!tryAgainMenu())
				return true;
		}
		else
			break;
	}

	/*Reads model file name*/
	while(1) {
		printf("Enter the name of the model file (or nothing for '%s/default.html'): ", MODEL_DIR);
		INPUT_CLEAR;
		
		if (! scanf("%[^\n]", model) ) {
			strcpy(model, "default.html");
		}
				
		if ( validateFile(model) == FILE_EXISTS ) {
			break;
		}

		tmp_model = pathCat(MODEL_DIR, model);

		if( validateFile(tmp_model) != FILE_EXISTS ) {
			printf("Model '%s', doesn't exist\n", tmp_model);
			free(tmp_model);
			if (!tryAgainMenu())
				return true;
		} else {
			strncpy(model, tmp_model, 2049);
			free(tmp_model);
			break;
		}
	}
	
	/* Reads output file name*/
	while (1) {
		printf("Type the name for output HTML file: ");
		INPUT_CLEAR;

		if( !scanf("%[^\n]", outputFile) ) {
			printf("Invalid Entry\n");
			if (!tryAgainMenu())
				return true;
		}
		else
			break;
	}
	
	/*Reads search keys*/
	if(!combinedSearchMenu(isbn, title, author, year, subject, type))
		return true;
	
	switch(type) {
		case ISBN:
			found_no = query(catalogName, isbn, ISBN, rrns);
			break;
		case TITLE:
			found_no = query(catalogName, title, TITLE, rrns);
			break;
		case AUTHOR:
			found_no = query(catalogName, author, AUTHOR, rrns);
			break;
		case SUBJECT:
			found_no = query(catalogName, subject, SUBJECT, rrns);
			break;
		case YEAR:
			found_no = query(catalogName, year, YEAR, rrns);
			break;
		case -1:
			found_no = queryKeyWords(catalogName, check_str(isbn), check_str(title), check_str(year), check_str(author), check_str(subject), rrns);
			break;
		default:
			fprintf(stderr, "Passed wrong argument to searchMenu()\n");
			exit(1);
	}
	
	/*Creates HTML file with results*/
	if (found_no == 0) {
			printf("No book matches search\n");
			return true;
	}
	if (found_no == -1) {
			return false;
	}
	if(!generateBooksDescription(rrns, catalogName, model, outputFile)) {
			return false;
	}
		
	return true;
}

