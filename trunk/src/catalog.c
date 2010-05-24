#include "tools.h"
#include "index.h"
#include <getopt.h>

int queryKeyWords(char * catalogName, char * isbn, char * title, char * year, char * author, char * subject, int * results) {
	
	/* The next two arrays matrix is used as follows:
	 * The first query stores its result in res[0]. The second query stores its result in res[1].
	 * The intersection of res[0] and res[1] is stored in res[0], and, then, all the next queries
	 * store their values in res[1], having res[0] and res[1] intersected in res[0].
	 * After n queries for n keys, the resulting res[0] contains all registers that match search.
	 */
	int res[2][10000];
	int first_step = true;
	int found_no = 0;
	
	/*As isbn is the primaryKey, if it exists, there's only one occurence*/
	if( !null(isbn) ){
		found_no = query(catalogName, isbn, ISBN, res[0]);
		goto success_exit;
		
	}
	if( !null(year) ){
		switch (first_step) {
			case true: 
				if ( (found_no = query(catalogName, year, YEAR, res[0])) == -1 )
					return -1;
				first_step = 0;
				break;
			case false:
				if ( (found_no = query(catalogName, year, YEAR, res[1])) == -1 )
					return -1;
				/*If the number of elements of intersection is 0, returns 0*/
				if ( (found_no = intersection(res[0], res[1], res[0], -1)) == 0) {
					goto success_exit;
				}
				break;
		}
	}
	if( !null(title) ){
		switch (first_step) {
			case true: 
				if ( (found_no = query(catalogName, title, TITLE, res[0])) == -1 )
					return -1;
				first_step = 0;
				break;
			case false:
				if ( (found_no = query(catalogName, title, TITLE, res[1])) == -1 )
					return -1;
				/*If the number of elements of intersection is 0, returns 0*/
				if ( (found_no = intersection(res[0], res[1], res[0], -1)) == 0) {
					goto success_exit;
				}
				break;
		}
	}
	if( !null(author) ){
		switch (first_step) {
			case true: 
				if ( (found_no = query(catalogName, author, AUTHOR, res[0])) == -1 )
					return -1;
				first_step = 0;
				break;
			case false:
				if ( (found_no = query(catalogName, author, AUTHOR, res[1])) == -1 )
					return -1;
				/*If the number of elements of intersection is 0, returns 0*/
				if ( (found_no = intersection(res[0], res[1], res[0], -1)) == 0) {
					goto success_exit;
				}
				break;
		}
	}
	if( !null(subject) ){
		switch (first_step) {
			case true: 
				if ( (found_no = query(catalogName, subject, SUBJECT, res[0])) == -1 )
					return -1;
				first_step = 0;
				break;
			case false:
				if ( (found_no = query(catalogName, subject, SUBJECT, res[1])) == -1 )
					return -1;
				/*If the number of elements of intersection is 0, returns 0*/
				if ( (found_no = intersection(res[0], res[1], res[0], -1)) == 0) {
					goto success_exit;
				}
				break;
		}
	}

	/*Checks if res[0] has the result of at least one query*/
	if (first_step) {
		/*Check if it's better to print the whole list*/
		fprintf(stderr, "Nonsense search. All parameters are NULL");
		return -1;
	}

	success_exit:
	/*Copies res[0] to results and finishes*/
	memcpy(results, res[0], sizeof(int)*10000);
	return found_no;
}
	
bool nonInteractiveRemoval(char * catalogName, int argc, char * argv[]) {
	int opt_index = 0;
	int opt, break_code = 0; 
	const char * short_opt = "A:t:S:i:y:";
	struct option long_opt[] = {
		{"author", 1, 0, 'A'},
		{"title", 1, 0, 't'},
		{"subject", 1, 0, 'S'},
		{"isbn", 1, 0, 'i'},
		{"year", 1, 0, 'y'},
		{0,0,0,0}
	};

	while( (NEXT_OPT != -1) && !break_code ) {
		switch (opt) {
			case 'A':
				/*Get author names querying, checking if list of found books contain the new ones for applying "AND" */
				printf("List books from author of optarg and asks for removal\n");
				break_code = 1;
				break;
			case 't':
				/*Does the same*/
				printf("List optarg titled book and asks for removal\n");
				break_code = 1;
				break;
			case 'S':
				/*Does the same*/
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
	int opt;
	int found_no = 0;
	const char * short_opt = "o:i:m:A:S:t:i:y:";
	char* catalogName = optarg;
	char * HTMLout = NULL;
	char * HTMLmodel = NULL;
	char * tmp_str = NULL;
	struct option long_opt[] = {
		{"out", 1, 0, 'o'},
		{"isbn", 1, 0, 'i'},
		{"model", 1, 0, 'm'},
		{"author", 1, 0, 'A'},
		{"subject", 1, 0, 'S'},
		{"title", 1, 0, 't'},
		{"year", 1, 0, 'y'},
		{0,0,0,0},
	};
	
	/*Starts search keys*/
	char * sk[5];
	sk[AUTHOR] = NULL;
	sk[ISBN] = NULL;
	sk[TITLE] = NULL;
	sk[SUBJECT] = NULL;
	sk[YEAR] = NULL;
	
	int rrns[10000];
	
	while( (NEXT_OPT != -1) ) {
		switch (opt) {
			case 'i':
				if( null(sk[ISBN]) ) {
					sk[ISBN] = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			case 'S':
				if( null(sk[SUBJECT]) ) {
					sk[SUBJECT] = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			case 'A':
				if( null(sk[AUTHOR]) ) {
					sk[AUTHOR] = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			case 'y':
				if( null(sk[YEAR]) ) {
					sk[YEAR] = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			case 't':
				if( null(sk[TITLE]) ) {
					sk[TITLE] = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			case 'o':
				if( null(HTMLout) ) {
					HTMLout = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			case 'm':
				if( null(HTMLmodel) ) {
					HTMLmodel = optarg;
					break;
				}
				else {
					DUPLICATED_PARAMETERS;
					return false;
				}
			default:
				invalidParameter(opt);
				return false;
		}
	}
	
	/*TODO: See if it's better to use defaults fore HTMLout and HTMLmodel if they're missing in argv*/
	if (HTMLout == NULL || HTMLmodel == NULL) {
		MISSING_ARGUMENTS;
		return false;
	}	

	if (validateFile(HTMLmodel) != FILE_EXISTS) {
		/* HTMLmodel may be relative to MODEL_DIR */
		if ( validateFile( tmp_str = pathCat(MODEL_DIR, HTMLmodel ) ) != FILE_EXISTS ) {
			fprintf(stderr, "Model file '%s' does not exist\n", tmp_str);
			free (tmp_str); return false;
		} else {
			found_no = queryKeyWords(catalogName, sk[ISBN], sk[TITLE], sk[YEAR], sk[AUTHOR], sk[SUBJECT], rrns);
			if (found_no == 0) {
				printf("No book matches search\n");
				return true;
			}
			if (found_no == -1) {
				return false;
			}
			if(!generateBooksDescription(rrns, catalogName, tmp_str, HTMLout)) {
				free(tmp_str);	
				return false;
			}
		}
	
	} else { /* HTMLmodel exists, use it */
		found_no = queryKeyWords(catalogName, sk[ISBN], sk[TITLE], sk[YEAR], sk[AUTHOR], sk[SUBJECT], rrns);
		if (found_no == 0) {
			printf("No book matches search\n");
			return true;
		}
		if (found_no == -1) {
			return false;
		}
		if(!generateBooksDescription(rrns, catalogName, HTMLmodel, HTMLout))
			return false;
	}
		
	return true;
}

int query(char* catalogName, char* key, enum IndexType type, int* results) {

	/*Must load two indexes. One for ISBN (primary key) and one for key (secondary key)*/

	/*Primary Index (ISBN)*/
	char * primary_idx_name = NULL;
	FILE * primary_idx_file = NULL;
	Index * primary_idx = NULL;

	/*Secondary Index (TYPE)*/
	char * secondary_idx_name = NULL;
	FILE * secondary_idx_file = NULL;
	Index * secondary_idx = NULL;
	
	/*Catalog*/
	FILE * catalog = NULL;
	
	/*Aux*/
	IndexEntry * match;
	int i=0, j=1;
	
	/*Checks if catalog exists - Returns false if not. Accesses it otherwise.*/
	if ( validateFile(catalogName) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist\n", catalogName);
		return -1;
	}
	if(null( (catalog = accessFile(catalogName, "r")) )) {
		return -1;
	}
	
	/*Type of primary index filename: "../idx/CATALOGOXXISBN.idx"*/
	primary_idx_name = IndexName(catalogName, ISBN);
	/*Checks if primary index exists - Creates idx if not.*/
	if (validateFile(primary_idx_name) != FILE_EXISTS) {
		if (!createIndex(catalogName, primary_idx_name, ISBN)) {
			fprintf(stderr, "Error: Indexing Error.\n");
			goto error_cleanup;
		}
	}
	/*Accesses Primary Index*/
	if(null(primary_idx_file = accessFile(primary_idx_name, "r"))) {
		fprintf(stderr, "Error: Failed to open index.\n");
		goto error_cleanup;
	}
	/*Loads Primary Index*/
	if( null( (primary_idx = loadIndex(primary_idx_file, ISBN)) ) ) {
		fprintf(stderr, "Error: Index Allocation Problem.\n");
		goto error_cleanup;
	}

	/* Checks if the key has the type of a Primary Key (ISBN).
	 * If it does, opening a secondary index is not necessary.
	 */
	if ( type != ISBN ) {
		/*Type of secondary index filename: "../idx/CATALOGOXXISBN.idx"*/
		secondary_idx_name = IndexName(catalogName, type);
		/*Checks if secondary index exists - Creates idx if not.*/
		if (validateFile(secondary_idx_name) != FILE_EXISTS) {
			if (!createIndex(catalogName, secondary_idx_name, type)) {
				fprintf(stderr, "Error: Indexing Error.\n");
				goto error_cleanup;
			}
		}
		/*Accesses Secondary Index*/
		if(null(secondary_idx_file = accessFile(secondary_idx_name, "r"))) {
			fprintf(stderr, "Error: Failed to open index.\n");
			goto error_cleanup;
		}
		/*Loads Secondary Index*/
		if( null( (secondary_idx = loadIndex(secondary_idx_file, type)) ) ) {
			fprintf(stderr, "Error: Index Allocation Problem.\n");
			goto error_cleanup;
		}
		/*Search for secondary key, transforming the resulting ISBNS in RRNS by searching for primary key*/
		switch( (i = searchIndex(secondary_idx, key, type)) ) {
			case -1:
				printf("%s doesn't exist in %s\n", key, catalogName);
			case -2:
				goto error_cleanup;
			default:
				match = getNextMatch(secondary_idx, type, key, i);
				results[0] = RRN(primary_idx->entries[searchIndex(primary_idx, (match->isbn), ISBN)]);
				while ( (match = getNextMatch(secondary_idx, type, NULL, -1)) ) {
					results[j++] = RRN(primary_idx->entries[searchIndex(primary_idx, (match->isbn), ISBN)]);
				}results[j] = -1;
		}
	}
	else {
		switch( (i = searchIndex(primary_idx, key, type)) ) {	
			case -1:
				printf("%s doesn't exist in %s\n", key, catalogName);
			case -2:
				goto error_cleanup;
			default:
				results[0] = RRN(primary_idx->entries[i]);
				results[1] = -1;
				j = 1;
		}
	}
	
	if( !null(catalog) ) fclose(catalog);
	
	if( !null(primary_idx_name) ) free(primary_idx_name);
	if( !null(primary_idx_file) ) fclose(primary_idx_file);
	if( !null(primary_idx) ) freeIndex(primary_idx);
	
	if( !null(secondary_idx_name) ) free(secondary_idx_name);
	if( !null(secondary_idx_file) ) fclose(secondary_idx_file);	
	if( !null(secondary_idx) ) freeIndex(secondary_idx);
	
	return j;
	
	error_cleanup:
		if( !null(catalog) ) fclose(catalog);
	
		if( !null(primary_idx_name) ) free(primary_idx_name);
		if( !null(primary_idx_file) ) fclose(primary_idx_file);
		if( !null(primary_idx) ) freeIndex(primary_idx);
	
		if( !null(secondary_idx_name) ) free(secondary_idx_name);
		if( !null(secondary_idx_file) ) fclose(secondary_idx_file);	
		if( !null(secondary_idx) ) freeIndex(secondary_idx);
	
	return -1;
}

bool generateList(char* CatalogName, int * rrns, char* HTMLlistName) {

	Book * pbook;
	FILE * catalog;
	FILE * idx_file;
	FILE * list;
	char * idx_name;
	Index * idx;
	int i;
	
	
	/*Checks if catalog exists - Returns false if not. Accesses it otherwise.*/
	if ( validateFile(CatalogName) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist\n", CatalogName);
		return false;
	}
	if(null( (catalog = accessFile(CatalogName, "r")) )) {
		return false;
	}
	
	/*Opens List*/
	list = fopen(HTMLlistName, "w+");
	if(null(list)) {
		fprintf(stderr, "Impossible to create HTML list. Allocation problem");
		fclose(catalog); return false;
	}
	
	if( rrns == NULL) {
		/*Catalogs default ISBN index name*/
		idx_name = IndexName(CatalogName, ISBN);

		/*Checks if index exists - Creates idx if not.*/
		if(validateFile(idx_name) != FILE_EXISTS) {
			if (! createIndex(CatalogName, idx_name, ISBN)) {
				fprintf(stderr, "Error: Indexing Error.\n");
				fclose(catalog); return false;
			}
		}
		if(null(idx_file = accessFile(idx_name, "r"))) {
			fprintf(stderr, "Error: Failed to open index.\n");
			free (idx_name); fclose(catalog); return false;
		}

		/*Loads Index*/
		idx = loadIndex(idx_file, ISBN);
		if(null(idx)) {
			fprintf(stderr, "Error: Index Allocation Problem.\n");
			free (idx_name); fclose(idx_file);fclose(catalog); return false;
		}
		startHTMLCatalogList(list);
	
		/*Creates Book*/
		pbook = createBook();

		/*Reads ISBNs, finding RRNS,appending books to List*/
		for(i = 0; i< idx->entries_no; i++) {
			seekRRN(catalog, *((int *) idx->entries[i].data));
			getNextBook(pbook, catalog);
			
		appendHTMLCatalogList(list, pbook);
		}
	
		finishHTMLCatalogList(list);
		printf("HTML list '%s' successfully created\n", HTMLlistName);
		free (idx_name); fclose(idx_file); fclose(catalog); freeIndex (idx);
		return true;

	}
	else {
	
		if (rrns[0] == -1) {
			fprintf(stderr, "Empty list of rrns. HTML list was note created.\n");
			fclose (catalog); return true;
		}
		
		startHTMLCatalogList(list);
	
		/*Creates Book*/
		pbook = createBook();

		for(i = 0 ; rrns[i] != -1; i++) {
			seekRRN(catalog, rrns[i]);
			getNextBook(pbook, catalog);

			appendHTMLCatalogList(list, pbook);
		}
		finishHTMLCatalogList(list);
		printf("HTML list '%s' successfully created\n", HTMLlistName);
		fclose (catalog); return true;
	}
}

bool generateBooksDescription(int * rrns, char * catalogName, char * modelFile, char * HTMLout) {
	FILE * model = NULL;
	FILE * bkdscr = NULL;
	FILE * catalog = NULL;
	Book * bk = NULL;
	char * img = NULL;
	char * imgPath = "img/";
	int i;
	
	/*Book's strings*/
	char isbn[ISBN_SIZE + 1];
	char title[TITLE_SIZE + 1];
	char author[AUTHOR_SIZE + 1];
	char subject[SUBJECT_SIZE + 1];
	char year[YEAR_SIZE + 1];
	char imgfile[IMGFILE_SIZE + 1];
	char summary[SUMMARY_SIZE + 1];
	char characters[CHARACTER_SIZE + 1];
	
	/*Checks if catalog exists - Returns false if not. Accesses it otherwise.*/
	if ( validateFile(catalogName) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist\n", catalogName);
		return false;
	}
	if(null( (catalog = accessFile(catalogName, "r")) )) {
		return false;
	}
		
	/*Tries to open model*/
	if(validateFile(modelFile) != FILE_EXISTS) {
		fprintf(stderr, "'%s', model for book description, doesn't exist\n", modelFile);
		return false;
	}
	model = accessFile(modelFile, "r");
	if(null(model)) {
		fprintf(stderr, "Problem opening model for book description\n");
		return false;

	}
	
	/*Creates Book*/
	if(!(bk = createBook())) {
		fprintf(stderr, "Impossible to get a book from catalog. Probably unavailable memory.");
		fclose(model); return false;
	}
	
	/*Creates book description based on model*/
	bkdscr = fopen(HTMLout, "w+");
	if(null(bkdscr)) {
		fprintf(stderr, "Problem creating book description file\n");
		goto error_cleanup;
	}
	
	
	/*Creates HTML file based on model containing all books from array RRNs*/
	for(i=0; rrns[i] != -1; i++) {
		seekRRN(catalog, rrns[i]);
		getNextBook(bk, catalog);
		
		/*Adequates strings of Book*/
		adqStr(bk->title, title, TITLE_SIZE);
		adqStr(bk->isbn, isbn, ISBN_SIZE);
		adqStr(bk->subject, subject , SUBJECT_SIZE);
		adqStr(bk->author, author, AUTHOR_SIZE);
		adqStr(bk->year, year, YEAR_SIZE);
		adqStr(bk->summary, summary, SUMMARY_SIZE);
		adqStr(bk->character, characters, CHARACTER_SIZE);
		adqStr(bk->imgfile, imgfile, IMGFILE_SIZE);

		/*Creates path string for image*/
		if(!(img = (char*)malloc( (strlen(imgPath) + strlen(imgfile) + 2)*sizeof(char) ))){
			fprintf(stderr, "Memory Allocation Problem\n");
			goto error_cleanup;
		}
		strcpy(img, imgPath);
		strncat(img, imgfile, strlen(imgfile) - 3);
		strcat(img, ".png");

		/*Replace special expressions in Model for Book Info. */
		expressionsReplacer(model, bkdscr, 8, "%TITLE", title, "%ISBN", isbn, "%SUBJECT", subject, "%AUTHOR", author, "%YEAR", year, "%SUMMARY", summary, "%CHARACTER",characters, "%IMAGE", img);
	}

	fclose(model);

	fclose(bkdscr);
	free(img);	
	fclose(catalog);
	printf("HTML Book Info %s successfully created\n", HTMLout);
	return true;
	
	error_cleanup:
		if(!null(model)) fclose(model);
		if(!null(catalog)) fclose(catalog);
		if(!null(bkdscr)) fclose(bkdscr);
		if(!null(img)) free(img);
		if(!null(bk)) free(bk);
		return false;
}

void appendHTMLCatalogList(FILE * list, Book * pbook) {
	fseek(list, 0, SEEK_END);
	
	char isbn[ISBN_SIZE + 1];
	char title[TITLE_SIZE + 1];
	char author[AUTHOR_SIZE + 1];
	char subject[SUBJECT_SIZE + 1];
	char year[YEAR_SIZE + 1];
	
	adqStr(pbook->isbn, isbn, ISBN_SIZE);
	adqStr(pbook->title, title, TITLE_SIZE);
	adqStr(pbook->author, author, AUTHOR_SIZE);
	adqStr(pbook->subject, subject, SUBJECT_SIZE);
	adqStr(pbook->year, year, YEAR_SIZE);

	fputs("\t\t\t<tr align = \"left\" valign = \"center\">\n", list);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", isbn);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", title);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", author);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", subject);
	fprintf(list, "\t\t\t\t<td>%s</td>\n", year);

	return;
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

bool removeBooks(int * rrns, char * catalogName) {
	printf("BOOKS REMOVED (not implemented)\n");
	return true;
}

