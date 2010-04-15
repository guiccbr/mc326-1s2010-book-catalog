#include <stdbool.h>

#define BOOK_BLOCK_SIZE 100

/* Appends a book record to a given catalog
 * Receives: Book * book - the book record
 * 	     FILE * catalog - pointer to an open catalog
 */
#define writeBook(book, catalog) fwrite((book), sizeof(Book), 1, (catalog))

/* Retrieves the next book in the catalog
 * Receives: Book * bookptr - A book pointer (pointing to enough memory) that
 * 			      will store the data read
 * 	     FILE * catalog - The book catalog
 */
#define getNextBook(bookptr, catalog) fread((bookptr), sizeof(Book), 1, (catalog))

/* Reads a block of BOOK_BLOCK_SIZE from a catalog.
 * Receives: Book * bookptr - A book pointer (pointing to enough memory) that
 * 			      will store the data read
 * 	     FILE * catalog - The book catalog
 */
#define readBlock(bookptr, catalog) fread((bookptr), sizeof(Book) * BOOK_BLOCK_SIZE, 1, (catalog))

/*Creates an HTML list of books within CatalogFile if CatalogFile exists,
* returning true. Otherwise, returns false.
*/
bool generateHTML(const char* CatalogFile);

/*Searches for keys in catalog*/
bool query();
