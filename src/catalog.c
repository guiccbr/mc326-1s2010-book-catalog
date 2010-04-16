#include "catalog.h"
#include "books.h"
#include "tools.h"

void fillISBNIndex(FILE * catalog, FILE * index) {
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

char ** loadISBNIndex(FILE * index) {
	int entries_no, i;
	/* Size of one entry (ISBN + RRN) */
	int entry_size = ISBN_SIZE * sizeof(char) + sizeof(int);
	char ** entries;

	NOT_IMPLEMENTED;
	return NULL; 
	
	/* Read the number of entries */
	fread(&entries_no, sizeof(int), 1, index);

	entries = (char ** ) malloc(entries_no * sizeof(char **));
	
	if (! entries ) return NULL;

	for ( i = 0; i < entries_no; i++ ) {
		entries[i] = (char *) malloc(entry_size);
		
		if (! entries[i] ) {
			while ( --i >= 0 ) free(entries[i]);
			free(entries);
			return NULL;
		}

		fread(entries[i], entry_size, 1, index);
	}

	return entries;
	
}

int searchISBNIndex(char ** index, char * isbn) {
	NOT_IMPLEMENTED;

	return 1;
}

bool query() {
	NOT_IMPLEMENTED;
	return true;
}

bool generateHTML(const char* CatalogFile) {
	NOT_IMPLEMENTED;
	return true;
}
