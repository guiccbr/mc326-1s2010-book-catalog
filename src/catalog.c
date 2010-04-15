#include "catalog.h"
#include "books.h"
#include "tools.h"

void fillISBNIndex(FILE * catalog, FILE * index) {
	Book block[BOOK_BLOCK_SIZE];
	int i, count = 0, read = readBlock(block, catalog);

	while (read) {
		for (i = 0; i < read; i++,count++ ) {
			fprintf(index, "%s %d\n", block[i].isbn, count);
		}

		read = readBlock(block, catalog);
	}
}

bool query() {
	NOT_IMPLEMENTED;
	return true;
}

bool generateHTML(const char* CatalogFile) {
	NOT_IMPLEMENTED;
	return true;
}
