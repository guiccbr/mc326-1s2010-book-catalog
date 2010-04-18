#include "index.h"
#include "catalog.h"
#include "tools.h"

bool createISBNIndex(char * catalog_file, char * index_file) {
	Book block[BOOK_BLOCK_SIZE];
	int i,read, count = 0;
	FILE * catalog, * index;

	if ( validateFile(catalog_file) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist!\n", catalog_file);
		return false;
	}

	catalog = accessFile(catalog_file, "r");

	if (! (index = openFile(index_file, "w")) ) return false;
	
	/* Skip space for recording the number of entries later */
	fseek(index, sizeof(int), SEEK_SET);
	
	read = readBlock(block, catalog);

	while (read) {
		for (i = 0; i < read; i++,count++ ) {
			fwrite(block[i].isbn, sizeof(char), 13, index);
			fwrite(&count, sizeof(int), 1, index);
		}

		read = readBlock(block, catalog);
	}

	/* Write the number of registries */
	fseek(index, 0, SEEK_SET);
	fwrite(&count, sizeof(int), 1, index);
	
	fclose(index); /* Make sure nothing gets lost */
	
	/* Reopen file for sorting */
	if (! (index = accessFile(index_file, "r+b")) ) return false;

	sortISBNIndexFile(index);

	fclose(index);
	fclose(catalog);
	
	return true;
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

int compareISBN(const void * e1, const void * e2) {
	return strncmp(((const IndexEntry *) e1)->isbn,
			((const IndexEntry *) e2)->isbn,
			ISBN_SIZE);
}

int searchISBNIndex(Index * idx, char * isbn) {
	IndexEntry * found;
	IndexEntry target;

	if (! validateISBN(isbn) ) {
		fprintf(stderr, "Tried to search invalid ISBN: %s\n", isbn);
		return -2;
	}

	/* Set up a target with the desired ISBN string for comparison */
	strncpy(target.isbn, isbn, ISBN_SIZE);
	
	found = binarySearch(idx->entries, idx->entries_no, ENTRY_SIZE, &target, compareISBN);	

	if (! found ) return -1;

	return found->rrn;
}

bool sortISBNIndexFile(FILE * index_file) {
	Index * idx;
	
	fseek(index_file, 0, SEEK_SET);

	idx = loadISBNIndex(index_file);

	if (! idx ) {
		fprintf(stderr, "Could not load index file.\n");
		return false;
	}

	sortIndexEntries(idx, compareISBN);
	dumpISBNIndex(idx, index_file);
	
	freeISBNIndex(idx);

	return true;
}
