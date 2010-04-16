#include "catalog.h"
#include "books.h"

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

/* Creates an index of ISBN fields and corresponding positions of
 * registries in a catalog.
 * Receives: FILE * catalog - The catalog file from which data will be read.
 *           FILE * index - The open index file.
 * Note: Does not seek back to the beginning of the catalog file
 */
void createISBNIndex(FILE * catalog, FILE * index);

/* Creates in memory a representation of a given index file.
 * Receives: FILE * idx - An open index file.
 * Returns: Index * - A pointer to a new index representation.
 */
Index * loadISBNIndex(FILE * idx);

/* Frees an ISBN index.
 * Receives: Index * idx - an index to be freed
 */
void freeISBNIndex(Index * idx);

/* Writes an ISBN index to a file.
 * Receives: Index * idx - the index.
 *           FILE * index - an open file descriptor to the index file.
*/
void dumpISBNIndex(Index * idx, FILE * idx_file);

/* Searches the index for a specific ISBN.
 * Receives: Index * idx - The index object.
 *           char * isbn - An ISBN string.
 * Returns: The RRN of the registry found, or -1 on error.
 */
int searchISBNIndex(Index * idx, char * isbn);
