#include "tools.h"
#include "index.h"
#include <getopt.h>

bool nonInteractiveRemoval(char * catalogName, int argc, char * argv[]) {
	int opt_index = 0;
	int opt, break_code = 0; 
	const char * short_opt = "A:t:S:i:";
	struct option long_opt[] = {
		{"author", 1, 0, 'A'},
		{"title", 1, 0, 't'},
		{"subject", 1, 0, 'S'},
		{"isbn", 1, 0, 'i'},
		{0,0,0,0}
	};

	while( (NEXT_OPT != -1) && !break_code ) {
		switch (opt) {
			case 'A':
				printf("List books from author of optarg and asks for removal\n");
				break_code = 1;
				break;
			case 't':
				printf("List optarg titled book and asks for removal\n");
				break_code = 1;
				break;
			case 'S':
				printf("List books whose subject is given by optarg\n");
				break_code = 1;
				break;
			case 'i':
				printf("List book of isbn equal to optarg and asks for removal\n");
			
			default:
				invalidParameter(opt);
				break_code = 1;
				break;
		}
	}
	if (!break_code) {
		MISSING_ARGUMENTS;
		return 0;
	}

	/*Removes Books*/
	return 1;
}


bool nonInteractiveQuery(int argc, char * argv[]) {

	int opt_index = 0;	
	int opt, break_code = 0, exit_code = 0;
	const char * short_opt = "o:i:m:";
	char* catalogName = optarg;
	char * isbn = NULL;
	char * HTMLout = NULL;
	char * HTMLmodel = NULL;
	char * tmp_str = NULL;
	struct option long_opt[] = {
		{"out", 1, 0, 'o'},
		{"isbn", 1, 0, 'i'},
		{"model", 1, 0, 'm'},
		{0,0,0,0},
	};
	
	while( (NEXT_OPT != -1) && !break_code ) {
		switch (opt) {
			case 'i':
				isbn = optarg;
				if( !null(isbn) && !null(HTMLout) && !null(HTMLmodel) )
					break_code = 1;
				break;
			case 'o':
				HTMLout = optarg;
				if( !null(isbn) && !null(HTMLout) && !null(HTMLmodel) )
					break_code = 1;
				break;
			case 'm':
				HTMLmodel = optarg;
				if( !null(isbn) && !null(HTMLout) && !null(HTMLmodel) )
					break_code = 1;
				break;
			default:
				invalidParameter(opt);
				exit_code = break_code = 1;
				break;
		}
	}
	
	if (!break_code) {
		MISSING_ARGUMENTS;
		return 0;
	}		
	
	if (exit_code) {
		return 0;
	}

	if (validateFile(HTMLmodel) != FILE_EXISTS) {
		/* HTMLmodel may be relative to MODEL_DIR */
		if ( validateFile( tmp_str = pathCat(MODEL_DIR, HTMLmodel ) ) != FILE_EXISTS ) {
			fprintf(stderr, "Model file '%s' does not exist\n", tmp_str);
			free (tmp_str); return false;
		} else {
			query(catalogName, isbn, HTMLout, tmp_str);
			free(tmp_str);
		}
	
	} else { /* HTMLmodel exists, use it */
		query(catalogName, isbn, HTMLout, HTMLmodel);
	}
		

	return true;
}

bool query(char* catalogName, char* primaryKey, char* HTMLout, char* InfoModel) {

	unsigned int rrn;
	char * idxName;
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
		if (!createIndex(catalogName, idxName, ISBN)) {
			fprintf(stderr, "Error: Indexing Error.\n");
			free (idxName); return false;
		}
	}
	if(null(idx = accessFile(idxName, "r"))) {
		fprintf(stderr, "Error: Failed to open index.\n");
		free (idxName); return false;
	}

	/*Loads index and Catalog*/
	index = loadIndex(idx, ISBN);
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
	switch( (rrn = searchIndex(index, primaryKey, ISBN)) ) {
		case -1:
			printf("%s doesn't exist in %s\n", primaryKey, catalogName);
			free (idxName); fclose(idx); fclose(catalog); return false;
		case -2:
			free (idxName); fclose(idx); fclose(catalog); return false;
	}
	rrn = RRN(index->entries[rrn]);
	seekRRN(catalog, rrn);

	/*Reads book*/
	foundBook = createBook();
	getNextBook(foundBook, catalog);

	/*Creates HTML book description from model*/
	generateBookDescription(foundBook, InfoModel, HTMLout);

	/*Frees dinamically allocated memory*/
	freeIndex(index);
	free(idxName);
	free(foundBook);

	/*Closes Files*/
	fclose(idx);
	fclose(catalog);
	
	return true;
}

bool generateList(char* CatalogName, char* HTMLlistName) {

	Book * pbook;
	FILE * catalog;
	FILE * catidx;
	FILE * list;
	char * idxName;
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
		if (! createIndex(CatalogName, idxName, ISBN)) {
			fprintf(stderr, "Error: Indexing Error.\n");
			free (idxName); fclose(catalog); return false;
		}
	}
	if(null(catidx = accessFile(idxName, "r"))) {
		fprintf(stderr, "Error: Failed to open index.\n");
		free (idxName);fclose(catalog); return false;
	}

	/*Loads Index and Catalog*/
	idx = loadIndex(catidx, ISBN);
	if(null(idx)) {
		fprintf(stderr, "Error: Index Allocation Problem.\n");
		free (idxName); fclose(catidx);fclose(catalog); return false;
	}

	/*Opens List*/
	list = fopen(HTMLlistName, "w+");
	if(null(list)) {
		fprintf(stderr, "Impossible to create HTML list. Allocation problem");
		free (idxName); fclose(catidx); fclose(catalog); return false;
	}
	
	startHTMLCatalogList(list);
	
	/*Creates Book*/
	pbook = createBook();

	/*Reads ISBNs, finding RRNS,appending books to List*/
	for(i = 0; i< idx->entries_no; i++) {
		seekRRN(catalog, *((int *) idx->entries[i].data));
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
	freeIndex(idx);
	free(pbook);	

	printf("HTML list '%s' successfully created\n", HTMLlistName);
	
	return true;
}

bool generateBookDescription(Book* bk, char* modelFile, char* HTMLout) {
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
	bkdscr = fopen(HTMLout, "w+");
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

	printf("HTML Book Info %s successfully created\n", HTMLout);

	return true;
}

void appendHTMLCatalogList(FILE * list, char * isbn, char * title) {
	fseek(list, 0, SEEK_END);

	fputs("\t\t\t<tr align = \"center\" valign = \"center\">\n", list);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", isbn);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", title);
	
}

void startHTMLCatalogList(FILE * list) {
	fseek(list, 0, SEEK_SET);

	fputs("<html>\n\n", list);
	fputs("\t<head>\n", list);
	fputs("\t<title>[MC326] Catalogo de Obras Literarias</title>\n", list);
	fputs("\t</head>\n\n", list);
	fputs("\t<body>\n", list);
	fputs("\t\t<table>\n", list);
	fputs("\t\t\t<tr align = \"center\" valign = \"center\">\n", list);
	fputs("\t\t\t\t<td><b>ISBN<b></td>\n", list);
	fputs("\t\t\t\t<td><b>Title<b></td>\n", list);

	return;
}

void finishHTMLCatalogList(FILE * list) {
	fseek(list, 0, SEEK_END);

	fputs("\t\t\t</tr>\n",list);
	fputs("\t\t</table>\n",list);
	fputs("\t</body>\n\n",list);
	fputs("</html>\n", list);

}

