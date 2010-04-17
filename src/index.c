#include "index.h"
#include "catalog.h"
#include "tools.h"

void createISBNIndex(FILE * catalog, FILE * index) {
	Book block[BOOK_BLOCK_SIZE];
	int i, count = 0, read = readBlock(block, catalog);

	fseek(index, sizeof(int), SEEK_SET);

	while (read) {
		for (i = 0; i < read; i++,count++ ) {
			fwrite(block[i].isbn, sizeof(char), 13, index);
			fwrite(&count, sizeof(int), 1, index);

			/*fprintf(index, " %d\n", count);*/
		}

		read = readBlock(block, catalog);
	}

	/* Write the number of registries */
	fseek(index, 0, SEEK_SET);
	fwrite(&count, sizeof(int), 1, index);
	
	fseek(index, 0, SEEK_SET);
	fseek(catalog, 0, SEEK_SET);

	return;
}

Index * loadISBNIndex(FILE * idx) {
	int i;
	Index * new_index;

	new_index = (Index *) malloc(sizeof(Index));
	if (! new_index ) return NULL;
	
	/* Read the number of entries */
	fread(&(new_index->entries_no), sizeof(int), 1, idx);

	/* Allocate enough space for all entries */
	new_index->entries = (IndexEntry *) malloc(new_index->entries_no * ENTRY_SIZE);
	
	if (! new_index->entries ) {
		fseek(idx, 0, SEEK_SET);
		free(new_index);
		return NULL;
	}

	for ( i = 0; i < new_index->entries_no; i++ ) {
		/* Read each entry to the Index */
		fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
		fread(&(new_index->entries[i].rrn), RRN_SIZE, 1, idx);
	}

	fseek(idx, 0, SEEK_SET);
	return new_index;
}

void freeISBNIndex(Index * idx) {
	free(idx->entries);
	free(idx);

	return;
}

void dumpISBNIndex(Index * idx, FILE * idx_file) {
	int i;

	/* Write the number of registries */
	fseek(idx_file, 0, SEEK_SET);
	fwrite(&idx->entries_no, sizeof(int), 1, idx_file);

	/* Write each registry */
	for ( i = 0; i < idx->entries_no; i++ ) {
		fwrite(idx->entries[i].isbn, ISBN_SIZE, 1, idx_file);
		fwrite(&(idx->entries[i].rrn), RRN_SIZE, 1, idx_file);
	}

	fseek(idx_file, 0, SEEK_SET);

	return;
}

int compareISBN(char * isbn1, char * isbn2) {
	return strncmp(isbn1, isbn2, ISBN_SIZE);
}

int searchISBNIndex(Index * idx, char * isbn) {
	IndexEntry * found;

	if (! validateISBN(isbn) ) {
		fprintf(stderr, "Tried to search invalid ISBN: %s\n", isbn);
		return -2;
	}

	found = binarySearch(idx->entries, idx->entries_no, ENTRY_SIZE, isbn, compareISBN);	

	if (! found ) return -1;

	return found->rrn;
}
