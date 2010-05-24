#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "catalog.h"

typedef struct {
	char isbn[ISBN_SIZE];
	void * data;
} IndexEntry;

typedef struct {
	unsigned int entries_no;
	IndexEntry * entries;
} Index;

/* Delimiter for words and ISBN entries in secondary indexes */
#define DELIMITER ":"
/* Format for reading words from secondary indexes with fscanf */
#define WORD_FORMAT "%[^" DELIMITER "]"

/* Get RRN field from an ISBN IndexEntry */
#define RRN(entry) *((unsigned int *) (entry).data)
#define RRN_FROM_PTR(entry) RRN(*(entry))

/* Get Word DATA for an IndexEntry as a char *;
 * used to retrieve year or word from an IndexEntry */
#define WDATA(entry) ((char *) ((entry).data))
#define WDATA_FROM_PTR(entry) YEAR(*(entry))

/* Max size for words in secondary indexes. 
 * Enough to fit in "Anticonstitucionalissimamente".
 */
#define WORD_MAX 30

#define INDEX_DIR "idx"

#define ENTRY_SIZE sizeof(IndexEntry)
#define RRN_SIZE sizeof(unsigned int)

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

/* Safely compares the year strings of two index entries.
 * Receives: const void * e1, * e2 - The index entries.
 * Returns: A negative, null or positive integer, respectively if e1->isbn is
 * "smaller" than, equal to, or "bigger" than e2->isbn.
 * Note: Does not check for malformed strings.
 */
int compareYear(const void * e1, const void * e2);

/* Safely compares the 'data' fields of two index entries when they contain
 * words (ie, the entries are of type SUBJECT, AUTHOR or TITLE)
 * Receives: const void * e1, * e2 - The index entries.
 * Returns: A negative, null or positive integer, respectively if e1->isbn is
 * "smaller" than, equal to, or "bigger" than e2->isbn.
 * Note: Does not check for malformed strings.
 */
int compareWords(const void * e1, const void * e2);

/* Creates an index out of a catalog.
 * Receives: char * catalog - The catalog file from which data will be read.
 *           char * index - The index file.
 *           enum IndexType type - The type of the index.
 * Returns: true or false upon success or error.
 */
bool createIndex(const char * catalog_file, char * index_file, enum IndexType type);

/* Creates in memory a representation of a given index file.
 * Receives: FILE * idx - An open index file.
 *           enum IndexType type - The type of the index.
 * Returns: Index * - A pointer to a new index representation.
 * Note: Seeks back to the beginning of the index file
 */
Index * loadIndex(FILE * idx, enum IndexType type);

/* Frees an index.
 * Receives: Index * idx - an index to be freed
 */
void freeIndex(Index * idx);

/* Writes an index to a file.
 * Receives: Index * idx - the index.
 *           FILE * index - an open file descriptor to the index file.
 * Returns: true or false as defined in stdbool.h
 * Note: Seeks back to the beginning of the index file after writing
 */
bool dumpIndex(Index * idx, FILE * idx_file, enum IndexType type);

/* Searches the index for a specific ISBN.
 * Receives: Index * idx - The index object.
 *           char * data - A '\0'-terminated ISBN, year or word string.
 *           enum IndexType type - The type of the index.
 * Returns: The index of the first match in idx->entries, -1 if none
 * was found or -2 on error.
 */
int searchIndex(Index * idx, char * data, enum IndexType type);

/* Loads an index file, sorts its contents and writes it back.
 * Receives: FILE * index_file - The FILE * corresponding to the index.
 *           enum IndexType type - The type of the index.
 * Returns: true or false upon success or error.
 * Note: Seeks back to the beginning of the index.
 */
bool sortIndexFile(FILE * index_file, enum IndexType type);

/* Creates a default name for idx file of a specific catalog.
 * Name and path of Catalog: 'cat.dat' - Name and path of Index: '/idx/cat.idx'
 * Receives:	char * catalogName - Name of the Catalog.
 *		char * TYPE - Type of the index (TITLE AUTHOR YEAR ISBN SUBJECT)
 * Returns: 	char * - Index Name.
 * Note: Return (Index Name) must be freed before program end.
 */
char * IndexName(const char * catalogName, enum IndexType type);

/* Splits a string into words, writing each word, DELIMITER and isbn to an
 * index file.
 * Receives: char * str - The string that will be split.
 * 	     int size - The size of str
 * 	     char * isbn - The isbn string that follows DELIMITER
 *	     FILE * index - The index file.
 * Returns: the number of words written.
 */
int writeWords(char * str, int size, char * isbn, FILE * index);

/* Returns the next IndexEntry that that matches a certain key. Should be initialized with
 * the first match and a key, and then called with NULL key until returns NULL, indicating
 * end of matches.
 * Receives: Index * idx - The index. Should not change between calls.
 * 	     enum IndexType type - The type of the index.
 * 	     char * key - The key that should match the IndexEntries.
 * 	     int firstmatch - The first match, as returned by searchIndex.
 * Notes: Changing keys between calls starts the process over
 * 	  Keeps a reference to the key. DO NOT free the key between calls.
 * 	  firstmatch is ignored when key is NULL.
 */
IndexEntry * getNextMatch(Index * idx, enum IndexType type, char * key, int firstmatch);
