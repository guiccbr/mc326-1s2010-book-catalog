#include <stdlib.h>
#include <string.h>

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

/* Safely compares two ISBN strings.
 * Receives: char * isbn1, isbn2 - The ISBN strings.
 * Returns: A negative, null or positive integer, respectively if isbn1 is
 * "smaller" than, equal to, or "bigger" than isbn2.
 * Note: Does not check for malformed strings.
 */
int compareISBN(char * isbn1, char * isbn2);

/* Creates an index of ISBN fields and corresponding positions of
 * registries in a catalog.
 * Receives: FILE * catalog - The catalog file from which data will be read.
 *           FILE * index - The open index file.
 * Note: Seeks back to the beginning of both the index file and catalog file
 */
void createISBNIndex(FILE * catalog, FILE * index);

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
