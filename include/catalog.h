#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <glob.h>
#include <unistd.h>

#include "books.h"

enum IndexType {
	ISBN,
	TITLE,
	SUBJECT,
	AUTHOR,
	YEAR
};

enum available {
	LAST_AVAILABLE,
	FIRST_AVAILABLE
};

#define MODEL_DIR "models"
#define BOOK_BLOCK_SIZE 100
#define HEADER_OFFSET 128
#define GRAVESTONE '*'

/* Macro that checks if book was not excluded from catalog.
 * Receives:	Book * book - pointer to book.
 * Returns 1 if book->title[0] == GRAVESTONE. False otherwise.
 */
#define validateBook(book) ((book)->title[0] != (GRAVESTONE))

/* Appends a book record to a given catalog
 * Receives: 	Book * book - the book record
 * 	     	FILE * catalog - pointer to an open catalog
 */
#define writeBook(book, catalog) fwrite((book), sizeof(Book), 1, (catalog))

/* Retrieves the next book in the catalog
 * Receives: 	Book * bookptr - A book pointer (pointing to enough memory) that
 * 			will store the data read
 * 	     	FILE * catalog - The book catalog
 */
#define getNextBook(bookptr, catalog) fread((bookptr), sizeof(Book), 1, (catalog))

/* Reads a block of BOOK_BLOCK_SIZE books from a catalog.
 * Receives: 	Book * bookptr - A book pointer (pointing to enough memory) that
 *			will store the data read
 * 	     	FILE * catalog - The book catalog
 */
#define readBlock(bookptr, catalog) fread((bookptr), sizeof(Book), BOOK_BLOCK_SIZE, (catalog))

/* Seeks a catalog to a given RRN.
 * Receives: 	FILE * catalog - The catalog file
 * 	     	int rrn - The desired RRN
 */
#define seekRRN(catalog, rrn) fseek((catalog), HEADER_OFFSET + (rrn) * BOOK_SIZE, SEEK_SET)

/* Creates an HTML books from a list of RRNS from Catalog.
 * Receives:	char * CatalogName - The name of the calog file
		char * rrns - List of Books that will be printed
 *		char * HTMLlistName - The name of the output list file
 * Returns true on sucess, false otherwise.
 * Note: If char * rrns is NULL, returns a list of all books of catalog sorted by ISBN.
 */
bool generateList(char* CatalogName, int * rrns, char* HTMLlistName);

/* Searches for books specified by a query.
 * Receives:	int argc
 * 		char * argv[]
 * Returns true on sucess, false otherwise.
 */
bool nonInteractiveQuery(int argc, char * argv[]);

/* Searches for primary keys in catalog, saving in 'int * results' the rrns that were found.
 * Receives:	char * catalogName - Name of the catalog
 *		char * key - Search key
 *		enum IndexType type - Key type (ISBN, AUTHOR, TITLE, SUBJECT, YEAR)
 *		int * results - Pre-Allocated array with enough space for all rrns that match search + 1.
 * Returns number of elements found in success. Returns -1 otherwise.
 * Note: Displays error message. Finishes results array with -1 as termination value, just like
 * '\0' for strings.
 */
int query(char * catalogName, char * key, enum IndexType type, int* results);

/* Generates an HTML page with book info based in model
 * Receives:	int * rrns - Struct Book with Book information.
 *				char * modelFile - HTML model, with special expressions to be replaced by info.
 * Returns true on success. False otherwise.
 */
bool generateBooksDescription(int * rrns, char * catalog, char * modelFile, char * HTMLout);

/* Appends a boook to HTML list of books.
 * Receives:	FILE * list - open list file
 * 		Book * pbook - Book ptr
 * Note: Points to the end of the stream list after returning.
 */
void appendHTMLCatalogList(FILE * list, Book * pbook);

/* Closes tags in HTML file.
 * Receives:	FILE * list - Open file of output list.
 * Note: Points to the end of the stream list after returning.
 */
void finishHTMLCatalogList(FILE * list);

/* Start HTML file, opening tags.
 * Receives:	FILE * list - Open file of output list.
 * Note: Points to the end of the stream list after returning.
 */
void startHTMLCatalogList(FILE * list);

/* Removes a book or a list of books from the catalog, according to user's desire.
 * Receives:	char * catalogName - Name of the catalog from where books will be removed.
 *		int argc - Shell's input number of arguments.
 *		char argv[] - Shell's input arguments.
 * Returns true in success. False otherwise.
 */
bool nonInteractiveRemoval(char * catalogName, int argc, char * argv[]);

/* Returns the number of occurences if something was found.
 * Returns 0 if nothing was
 * Returns -1 in case of error
 * Array of results must be passed as parameter.
 * End of array of results is denoted by -1 (as \0 of strings).
 */
int queryKeyWords(char * cataloName, char * isbn, char * title, char * year, char * author, char * subject, int * results);

/* Removes books whose rrns are in (int * rrns) from catalog.
 * Receives:	char * catalogName - Name of the catalog.
 *		int * rrns - rrns array of books to be removed.
 * Returns true in success. False otherwise.
 * Note: Displays error message.
 */
bool removeBooks(char * catalogName, int * rrns);

/* Removes the book whose first char is pointed by the position of catalog stream.
 * Receives:	FILE * catalog - The position of this stream must point to the book that will be removed.
 *		char gravestone - Deletion character.
 * Note: Prints error message.
 */
void removeNextBook(FILE * catalog, char gravestone);

/* Seeks list of availability.
 * Receives: 	FILE * catalog - Stream catalog.
 * 		enum available dest - Destination of seeking.
 * Note: 	If dest == LAST_AVAILABLE, seeks until finds the last available field for a book.
 *		If dest == FIRST_AVAILABLE, seeks once, to the first available field.
 *		Prints error message.
 */
void seekAvailables(FILE * catalog, enum available dest);

/* Adds a book to the first available field in catalog.
 * Receives:	FILE * catalog - open catalog file.
 *		Book * book - pointer to book.
 * Returns true in success, false otherwise.
 * Note: Displays error message.
 */
bool addBook(FILE * catalog, Book * newbook);
