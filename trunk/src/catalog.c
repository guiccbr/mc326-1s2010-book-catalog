#include "tools.h"
#include "index.h"

bool query(char* catalogName, char* primaryKey) {

	int rrn;
	char * idxName;
	char * defaultDescriptionModel = "models/default.html";
	char * idxPath = "idx/";
	FILE * catalog;
	FILE * idx;
	Index * index;
	Book * foundBook;

	/*Type of default index filename: "../idx/CATALOGOXX.dat.idx" */
	idxName = (char*)malloc(sizeof(strlen(catalogName)) + sizeof(idxPath) + 1);
	if ( idxName == NULL ) {
		fprintf(stderr, "Allocation Problem\n");
		return false;
	}
	strcpy(idxName, idxPath);
	strcat(idxName, catalogName);
	/*Changes extension from .dat to .idx*/
	strcpy(strstr(idxName, ".dat"), ".idx");

	/*Checks if catalog exists - Returns false if not.*/
	if ( validateFile(catalogName) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist\n", catalogName);
		return false;
	}

	/*Checks if index exists - Creates idx if not.*/
	if (validateFile(idxName) != FILE_EXISTS) {
		if (!createISBNIndex(catalogName, idxName)) {
			fprintf(stderr, "Error: Indexing Error.\n");
			return false;
		}
	}
	if(null(idx = accessFile(idxName, "r"))) {
		fprintf(stderr, "Error: Failed to open index.\n");
		return false;
	}

	/*Loads index and Catalog*/
	index = loadISBNIndex(idx);
	if(null(index)) {
		fprintf(stderr, "Error: Index Allocation Problem.\n");
		return false;
	}
	catalog = accessFile(catalogName, "r");
	if(null(catalog))
		return false;

	/*Search*/
	switch( (rrn = searchISBNIndex(index, primaryKey)) ) {
		case -1:
			printf("%s doesn't exist in %s\n", primaryKey, catalogName);
			return false;
		case -2:
			return false;
	}
	seekRRN(catalog, rrn);

	/*Reads book*/
	foundBook = createBook();
	getNextBook(foundBook, catalog);

	/*Creates HTML book description from model*/
	if (!generateBookDescription(foundBook, defaultDescriptionModel));
	return false;

	/*Frees dinamically allocated memory*/
	free(index);
	free(idxName);
	free(foundBook);

	return true;
}

bool generateHTML(const char* CatalogFile) {
	NOT_IMPLEMENTED;
	return true;
}

bool generateBookDescription(Book* bk, char* modelFile) {
	FILE * model;
	FILE * bkdscr;
	char * BookSubs[8];
	char * imgPath = "img/";
	char * img;

	/*Creates temporary strings for Book Info*/
	if (
		!(BookSubs[0] = adqStr(bk->title, TITLE_SIZE)) ||
		!(BookSubs[1] = adqStr(bk->isbn, ISBN_SIZE)) ||
		!(BookSubs[2] = adqStr(bk->subject, SUBJECT_SIZE)) ||
		!(BookSubs[3] = adqStr(bk->author, AUTHOR_SIZE)) ||
		!(BookSubs[4] = adqStr(bk->year, YEAR_SIZE)) ||
		!(BookSubs[5] = adqStr(bk->summary, SUMMARY_SIZE)) ||
		!(BookSubs[6] = adqStr(bk->character, CHARACTER_SIZE)) ||
		!(BookSubs[7] = adqStr(bk->imgfile, IMGFILE_SIZE))
		) return false;

	/*Creating path string for image*/
	if(!(img = (char*)malloc( (strlen(imgPath) + strlen(BookSubs[7]) + 2)*sizeof(char) ))){
		fprintf(stderr, "Memory Allocation Problem\n");
		return false;
	}
	strcpy(img, imgPath);
	strncat(img, BookSubs[7], strlen(BookSubs[7]) - 3);
	strcat(img, ".png");

	/*Opens model*/
	if(validateFile(modelFile) != FILE_EXISTS) {
		fprintf(stderr, "'%s', model for book description, doesn't exist\n", modelFile);
		free(img); return false;
	}
	model = accessFile(modelFile, "r");
	if(null(model)) {
		fprintf(stderr, "Problem opening model for book description\n");
		free(img); return false;
	}

	/*Creates book description based on model*/
	bkdscr = fopen("bkdscr.html", "w+");
	if(null(bkdscr)) {
		fprintf(stderr, "Problem creating book description file\n");
		free(img); fclose(model); return false;
	}


	if(!expressionsReplacer(model, bkdscr, 8, "%TITLE",BookSubs[0], "%ISBN", BookSubs[1], "%SUBJECT", BookSubs[2], "%AUTHOR", BookSubs[3], "%YEAR", BookSubs[4], "%SUMMARY", BookSubs[5], "%CHARACTER",BookSubs[6], "%IMAGE", img));
	free(img); fclose(model); fclose(bkdscr); return false;

	fclose(model);
	fclose(bkdscr);

	system("firefox bkdscr.html");

	return true;
}
