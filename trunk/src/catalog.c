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
			fprintf(index, " %d\n", count);
		}

		read = readBlock(block, catalog);
	}

	/* Write the number of registries */
	fseek(index, 0, SEEK_SET);
	fwrite(&count, sizeof(int), 1, index);

	return;
}

char ** loadISBNIndex(FILE * index) {
	int size, i;
	char ** entries;

	NOT_IMPLEMENTED;
	return NULL; 
	
	/* Read the number of entries */
	fread(&size, sizeof(int), 1, index);

	entries = (char ** ) malloc(size * sizeof(char **));

	for ( i = 0; i < size; i++ ) {
		/* Allocate space of entries[i] */
		fscanf(index, "%[^\n]", entries[i]);

		fgetc(index); /* get rid of the newline character */
	}
	
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
