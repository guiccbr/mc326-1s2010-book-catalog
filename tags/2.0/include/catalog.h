#include <stdio.h>
#include <stdbool.h>
#include "books.h"

#define BOOK_BLOCK_SIZE 100

/* Appends a book record to a given catalog
 * Receives: 	Book * book - the book record
 * 	     		FILE * catalog - pointer to an open catalog
 */
#define writeBook(book, catalog) fwrite((book), sizeof(Book), 1, (catalog))

/* Retrieves the next book in the catalog
 * Receives: 	Book * bookptr - A book pointer (pointing to enough memory) that
 * 			    will store the data read
 * 	     		FILE * catalog - The book catalog
 */
#define getNextBook(bookptr, catalog) fread((bookptr), sizeof(Book), 1, (catalog))

/* Reads a block of BOOK_BLOCK_SIZE books from a catalog.
 * Receives: 	Book * bookptr - A book pointer (pointing to enough memory) that
 *				will store the data read
 * 	     		FILE * catalog - The book catalog
 */
#define readBlock(bookptr, catalog) fread((bookptr), sizeof(Book), BOOK_BLOCK_SIZE, (catalog))

/* Seeks a catalog to a given RRN.
 * Receives: 	FILE * catalog - The catalog file.
 * 	     		int rrn - The desired RRN.
 */
#define seekRRN(catalog, rrn) fseek((catalog), (rrn) * BOOK_SIZE, SEEK_SET)

/*Creates an HTML list of books within CatalogFile if CatalogFile exists,
 * returning true. Otherwise, returns false.
 */
bool generateList(char* CatalogName, char* HTMLlistName);

bool nonInteractiveQuery(int argc, char * argv[]);

/* Searches for primary keys in catalog
 * Receives:	char * catalogName - Name of the catalog user wants to find a book
 *		char * primaryKey - Search primary key
 *		char * HTMLout - Name of the output file
 *		char * HTMLModel - Name of the model file
 * Returns true on success, and false if key wasn't found or if a problem occurred.
 * Note: Displays error message.
 */
bool query(char* catalogName, char* primaryKey, char* HTMLout, char * HTMLModel);

/* Generates an HTML page with book info based in model
 * Receives:	Book * bk - Struct Book with Book information.
 *				char * modelFile - HTML model, with special expressions to be replaced by info.
 * Returns true on success. False otherwise.
 */
bool generateBookDescription(Book* bk, char* modelFile, char* HTMLout);

void appendHTMLCatalogList(FILE * list, char * isbn, char * title);

void finishHTMLCatalogList(FILE * list);

void startHTMLCatalogList(FILE * list);