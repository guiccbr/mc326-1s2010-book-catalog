#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "catalog.h"

typedef struct {
	char isbn[ISBN_SIZE];
	int rrn;
} IndexEntry;

typedef struct {
	int entries_no;
	IndexEntry * entries;
} Index;

#define ENTRY_SIZE sizeof(IndexEntry)
#define RRN_SIZE sizeof(int)

/* Sorts the index entries in an index.
* Receives: Index * idx - The index.
*           int (* cmp) () - A pointer to a function that compares two IndexEntry objects.
*/
#define sortIndexEntries(idx, cmp) qsort((idx)->entries, (idx)->entries_no, ENTRY_SIZE, (cmp));

/* Safely compares the ISBN strings of two index entries.
* Receives: const void * e1, * e2 - The index entries.
* Returns: A negative, null or positive integer, respectively if e1->isbn is
* "smaller" than, equal to, or "bigger" than e2->isbn.
* Note: Does not check for malformed strings.
*/
int compareISBN(const void * e1, const void * e2);

/* Creates a sorted index of ISBN fields and corresponding positions of
* registries in a catalog.
* Receives: char * catalog - The catalog file from which data will be read.
*           char * index - The index file.
* Returns: true or false upon success or error.
*/
bool createISBNIndex(const char * catalog_file, char * index_file);

/* Creates in memory a representation of a given index file.
* Receives: FILE * idx - An open index file.
* Returns: Index * - A pointer to a new index representation.
* Note: Seeks back to the beginning of the index file
*/
Index * loadISBNIndex(FILE * idx);

/* Frees an ISBN index.
* Receives: Index * idx - an index to be freed
*/
void freeISBNIndex(Index * idx);

/* Writes an ISBN index to a file.
* Receives: Index * idx - the index.
*           FILE * index - an open file descriptor to the index file.
* Note: Seeks back to the beginning of the index file after writing
*/
void dumpISBNIndex(Index * idx, FILE * idx_file);

/* Searches the index for a specific ISBN.
* Receives: Index * idx - The index object.
*           char * isbn - A '\0'-terminated ISBN string.
* Returns: The RRN of the registry found, -1 if none was found or -2 on error.
*/
int searchISBNIndex(Index * idx, char * isbn);

/* Loads an ISBN index file, sorts its contents by ISBN and writes it back.
* Receives: FILE * index_file - The FILE * corresponding to the index.
* Returns: true or false upon success or error.
* Note: Seeks back to the beginning of the index.
*/
bool sortISBNIndexFile(FILE * index_file);

/* Creates a default name for idx file of a specific catalog.
 * Name and path of Catalog: 'cat.dat' - Name and path of Index: '/idx/cat.idx'
 * Receives:	char * catalogName - Name of the Catalog.
 * Returns: 	char * - Index Name.
 * Note: Return (Index Name) must be freed before program end.
 */
char * ISBNIndexName(const char * catalogName);