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
		free(new_index);
		return NULL;
	}

	for ( i = 0; i < new_index->entries_no; i++ ) {
		/* Read each entry to the Index */
		fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
		fread(&(new_index->entries[i].rrn), RRN_SIZE, 1, idx);
	}

	return new_index;
}

void freeISBNIndex(Index * idx) {
	free(idx->entries);
	free(idx);

	return;
}

void dumpISBNIndex(Index * idx, FILE * idx_file) {
	NOT_IMPLEMENTED;
	return;
}

int searchISBNIndex(Index * idx, char * isbn) {
	NOT_IMPLEMENTED;
	return 0;
}
