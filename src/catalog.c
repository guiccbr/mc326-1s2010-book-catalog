#include "tools.h"
#include "index.h"

bool query(char* catalogName, char* primaryKey) {

	int rrn;
	char * idxName;
	char * defaultDescriptionModel = "models/default.html";
	FILE * catalog;
	FILE * idx;
	Index * index;
	Book * foundBook;

	if( !(strlen(primaryKey) == ISBN_SIZE && validateISBN(primaryKey)) ) {
		fprintf(stderr, "Tried to set invalid ISBN: %s\n", primaryKey);
		return false;
	}

	/*Checks if catalog exists - Returns false if not.*/
	if ( validateFile(catalogName) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist\n", catalogName);
		return false;
	}
	
	/*Type of default index filename: "../idx/CATALOGOXXISBN.idx"*/
	idxName = ISBNIndexName(catalogName);

	/*Checks if index exists - Creates idx if not.*/
	if (validateFile(idxName) != FILE_EXISTS) {
		if (!createISBNIndex(catalogName, idxName)) {
			fprintf(stderr, "Error: Indexing Error.\n");
			free (idxName); return false;
		}
	}
	if(null(idx = accessFile(idxName, "r"))) {
		fprintf(stderr, "Error: Failed to open index.\n");
		free (idxName); return false;
	}

	/*Loads index and Catalog*/
	index = loadISBNIndex(idx);
	if(null(index)) {
		fprintf(stderr, "Error: Index Allocation Problem.\n");
		free (idxName); fclose(idx); return false;
	}
	catalog = accessFile(catalogName, "r");
	if(null(catalog)) {
		free (idxName);
		fclose(idx);
		return false;
	}

	/*Search*/
	switch( (rrn = searchISBNIndex(index, primaryKey)) ) {
		case -1:
			printf("%s doesn't exist in %s\n", primaryKey, catalogName);
			free (idxName); fclose(idx); fclose(catalog); return false;
		case -2:
			free (idxName); fclose(idx); fclose(catalog); return false;
	}
	seekRRN(catalog, rrn);

	/*Reads book*/
	foundBook = createBook();
	getNextBook(foundBook, catalog);

	/*Creates HTML book description from model*/
	generateBookDescription(foundBook, defaultDescriptionModel);

	/*Frees dinamically allocated memory*/
	freeISBNIndex(index);
	free(idxName);
	free(foundBook);

	/*Closes Files*/
	fclose(idx);
	fclose(catalog);
	
	return true;
}

bool generateList(const char* CatalogName) {

	Book * pbook;
	FILE * catalog;
	FILE * catidx;
	FILE * list;
	char * idxName;
	char * listName = "BooksList.html";
	char * isbn;
	char * title;
	Index * idx;
	int i;

	if(validateFile(CatalogName) != FILE_EXISTS) {
		fprintf(stderr, "'%s' doesn't exist\n", CatalogName);
		return false;
	} catalog = accessFile(CatalogName, "r");
	if(null(catalog))
		return false;

	/*Catalogs default ISBN index name*/
	idxName = ISBNIndexName(CatalogName);

	/*Checks if index exists - Creates idx if not.*/
	if(validateFile(idxName) != FILE_EXISTS) {
		if (!createISBNIndex(CatalogName, idxName)) {
			fprintf(stderr, "Error: Indexing Error.\n");
			free (idxName); fclose(catalog); return false;
		}
	}
	if(null(catidx = accessFile(idxName, "r"))) {
		fprintf(stderr, "Error: Failed to open index.\n");
		free (idxName);fclose(catalog); return false;
	}

	/*Loads Index and Catalog*/
	idx = loadISBNIndex(catidx);
	if(null(idx)) {
		fprintf(stderr, "Error: Index Allocation Problem.\n");
		free (idxName); fclose(catidx);fclose(catalog); return false;
	}

	/*Opens List*/
	list = fopen(listName, "w+");
	if(null(list)) {
		fprintf(stderr, "Impossible to create HTML list. Allocation problem");
		free (idxName); fclose(catidx); fclose(catalog); return false;
	}
	
	startHTMLCatalogList(list);
	
	/*Creates Book*/
	pbook = createBook();

	/*Reads ISBNs, finding RRNS,appending books to List*/
	for(i = 0; i< idx->entries_no; i++) {
		seekRRN(catalog, idx->entries[i].rrn);
		getNextBook(pbook, catalog);
		
		isbn = adqStr(pbook->isbn, ISBN_SIZE);
		title = adqStr(pbook->title, TITLE_SIZE);
		
		appendHTMLCatalogList(list, isbn, title);
		free(isbn); free(title);
	}
	
	finishHTMLCatalogList(list);

	free (idxName); 
	fclose(catidx); 
	fclose(list);
	fclose(catalog);
	freeISBNIndex(idx);
	free(pbook);	

	printf("HTML list '%s' successfully created\n", listName);
	
	return true;
}

bool generateBookDescription(Book* bk, char* modelFile) {
	FILE * model;
	FILE * bkdscr;
	char * BookSubs[8];
	char * imgPath = "img/";
	char * img;
	int i;

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
		) return false; /*MEmory LEAK*/

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

	/*Replace special expressions in Model for Book Info. */
	expressionsReplacer(model, bkdscr, 8, "%TITLE",BookSubs[0], "%ISBN", BookSubs[1], "%SUBJECT", BookSubs[2], "%AUTHOR", BookSubs[3], "%YEAR", BookSubs[4], "%SUMMARY", BookSubs[5], "%CHARACTER",BookSubs[6], "%IMAGE", img);

	fclose(model);
	fclose(bkdscr);
	free(img);
	for(i=0; i<8; i++) {
		free(BookSubs[i]);
	}

	printf("HTML Book Info 'bkdscr.html' successfully created\n");

	return true;
}

void appendHTMLCatalogList(FILE * list, char * isbn, char * title) {
	fseek(list, 0, SEEK_END);

	fputs("<tr align = \"center\" valign = \"center\">\n", list);
	fprintf(list, "<td>%s</td>\n", isbn);
	fprintf(list, "<td>%s</td>\n", title);
	
}
void startHTMLCatalogList(FILE * list) {
	fseek(list, 0, SEEK_SET);

	fputs("<html>\n", list);
	fputs("<head>\n", list);
	fputs("<title>[MC326] Catalogo de Obras Literarias</title>\n", list);
	fputs("<body>\n", list);
	fputs("<table>\n", list);
	fputs("<tr align = \"center\" valign = \"center\">\n", list);
	fputs("<td><b>ISBN<b></td>\n", list);
	fputs("<td><b>Title<b></td>\n", list);

	return;
}
void finishHTMLCatalogList(FILE * list) {
	fseek(list, 0, SEEK_END);

	fputs("</tr>\n",list);
	fputs("</table>\n",list);
	fputs( "</body>\n",list);
	fputs("</html>\n", list);

}

