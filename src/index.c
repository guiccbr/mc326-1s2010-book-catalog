#include "index.h"
#include "tools.h"

IndexEntry * getNextMatch(Index * idx, enum IndexType type, char * key, int firstmatch) {
	static int match_state = -1;
	static char * key_state = NULL;
	IndexEntry * result = NULL;

	/* Start over */
	if ( (key) && (key_state != key) ) {
		key_state = key;
		match_state = firstmatch;
		return &idx->entries[match_state];
	}

	if ( ++match_state >= idx->entries_no ) {
		key_state = NULL;
		match_state = -1;
		return NULL;
	}	

	switch ( type ) {
		case ISBN:
			if (! compareISBN(idx->entries[match_state].isbn, key_state) )
				result = &idx->entries[match_state];
			break;
		case YEAR:
			if (! strncmp(WDATA(idx->entries[match_state]), key_state, YEAR_SIZE) )
				result = &idx->entries[match_state];
			break;
		default:
			if (! strncasecmp(WDATA(idx->entries[match_state]), key_state, WORD_MAX) )
				result = &idx->entries[match_state];
			break;
	}

	if (! result ) {
		key_state = NULL;
		match_state = -1;
	}

	return result;
}

int writeWords(char * str, int str_size, char * isbn, FILE * index) {
	char * fixed_string;
	char * word;
	int size, count = 0;

	if (! (fixed_string = appendNULL(str, str_size)) )
		return 0;

	word = strtok(fixed_string, " ");
	
	while ( word ) {
		count++;
		size = strlen(word);

		if ( size > WORD_MAX ) size = WORD_MAX;
		
		if ( fwrite(word, sizeof(char), size, index) < size ) goto return_error;
		if ( fputc(DELIMITER, index) == EOF ) goto return_error;
		if ( fwrite(isbn, sizeof(char), ISBN_SIZE, index) < ISBN_SIZE ) goto return_error;

		word = strtok(NULL, " ");
	}

	free(fixed_string);
	return count;

	/* Not reached unless from goto statements above. Print an error message and quit */
	return_error:
		free(fixed_string);
		fprintf(stderr, "Couldn't write word to index file!\n");
		return false;
}

bool createIndex(const char * catalog_file, char * index_file, enum IndexType type) {
	Book block[BOOK_BLOCK_SIZE];
	int i,read, wrote;
	FILE * catalog, * index;
	
	/* Tracks the number of entries in the catalog.
	 * For ISBN and year indexes, this is the same as the number of books
	 * in the catalog.
	 * However, for title, author and subject indexes, each Book yields
	 * multiple entries: each word in the relevant field. So we must count
	 * the number of words written, as each will later be an IndexEntry.
	 */
	unsigned int count = 0;

	if ( validateFile(catalog_file) != FILE_EXISTS ) {
		fprintf(stderr, "Catalog %s doesn't exist!\n", catalog_file);
		return false;
	}

	catalog = accessFile(catalog_file, "r");
	fseek(catalog, HEADER_OFFSET, SEEK_SET);

	if (! (index = openFile(index_file, "w")) ) {
		fclose(catalog);
		return false;
	}

	/* Skip space for recording the number of entries later */
	fseek(index, sizeof(int), SEEK_SET);

	read = readBlock(block, catalog);

	while (read) {
		/* Write data from each book into the index */
		for (i = 0; i < read; i++ ) {
			switch (type) {
				case ISBN: /* ISBN indexes relate ISBNs to RRNS */
					
					if ( fwrite(block[i].isbn, sizeof(char), ISBN_SIZE, index) < ISBN_SIZE ) {
						fclose(index); fclose(catalog);
						return false;
					}
					
					if ( fwrite(&count, sizeof(count), 1, index) < 1 ) {
						fclose(index); fclose(catalog);
						return false;
					}
					
					count++;
					
					break;
				
				case YEAR: /* Year indexes relate years to ISBNs */
					if ( fwrite(block[i].year, sizeof(char), YEAR_SIZE, index) < YEAR_SIZE ) {
						fclose(index); fclose(catalog);
						return false;
					}
					
					if ( fwrite(block[i].isbn, sizeof(char), ISBN_SIZE, index) < ISBN_SIZE ) {
						fclose(index); fclose(catalog);
						return false;
					}
					
					count++; 
					
					break;
				
				case TITLE: /* All other indexes relate words in their field to an ISBN */
					if (! (wrote = writeWords(block[i].title, TITLE_SIZE, block[i].isbn, index)) ) {
						fclose(index); fclose(catalog);
						return false;
					}

					count += wrote;

					break;
				
				case SUBJECT:
					if (! (wrote = writeWords(block[i].subject, SUBJECT_SIZE, block[i].isbn, index)) ) {
						fclose(index); fclose(catalog);
						return false;
					}

					count += wrote;
					
					break;
				
				case AUTHOR:
					if (! (wrote = writeWords(block[i].author, AUTHOR_SIZE, block[i].isbn, index)) ) {
						fclose(index); fclose(catalog);
						return false;
					}

					count += wrote;
					
					break;
			}
		}

		/* Read next batch of books and start over */
		read = readBlock(block, catalog);
	}

	if (! feof(catalog) ) { /* Error! We should have read every book in the catalog */
		fprintf(stderr, "Error while reading from catalog!\n");
		fclose(index); fclose(catalog);
		return false;
	}

	/* Write the number of registries */
	fseek(index, 0, SEEK_SET);
	if ( fwrite(&count, sizeof(count), 1, index) < 1 ) {
		fclose(index); fclose(catalog);
		return false;
	}

	fclose(index); /* Make sure nothing gets lost */

	/* Reopen file for sorting */
	if (! (index = accessFile(index_file, "r+b")) ) return false;

	sortIndexFile(index, type);

	fclose(index);

	fclose(catalog);

	return true;
}

Index * loadIndex(FILE * idx, enum IndexType type) {
	int i, j, c;
	Index * new_index;

	new_index = (Index *) malloc(sizeof(Index));
	if (! new_index ) return NULL;

	/* Read the number of entries */
	fread(&(new_index->entries_no), sizeof(unsigned int), 1, idx);

	/* Allocate enough space for all entries */
	new_index->entries = (IndexEntry *) malloc(new_index->entries_no * ENTRY_SIZE);

	if (! new_index->entries ) {
		fseek(idx, 0, SEEK_SET);
		free(new_index);
		return NULL;
	}

	for ( i = 0; i < new_index->entries_no; i++ ) {
		/* Read each entry to the Index */
		switch (type) {
			case ISBN:
				new_index->entries[i].data = malloc(sizeof(unsigned int));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
				fread(new_index->entries[i].data, RRN_SIZE, 1, idx);
				break;
			
			case YEAR:
				new_index->entries[i].data = malloc(YEAR_SIZE * sizeof(char));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].data, YEAR_SIZE, 1, idx);
				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
				break;
			
			default:
				/* Words are stored in the index separated by DELIMITER from their
				 * corresponding ISBN. In memory, they should be '\0'-terminated */
				new_index->entries[i].data = malloc((WORD_MAX+1) * sizeof(char));
				if (! new_index->entries[i].data ) goto error_cleanup;

				/* Read char by char and truncate to WORD_MAX
				 * to prevent buffer overflow */
				c = fgetc(idx);
				j = 0;
				while ( (c != DELIMITER) && (c != EOF) ) {
					if ( j < WORD_MAX ) {
						WDATA(new_index->entries[i])[j] = c;
						j++;
					}
					c = fgetc(idx);
				}

				WDATA(new_index->entries[i])[j] = '\0';

				/* Read ISBN */
				if ( fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx) < 1 ) {
					fprintf(stderr, "Couldn't read index entry!\n");
					return NULL;
				}
				break;
		}
	}

	fseek(idx, 0, SEEK_SET);
	return new_index;

	error_cleanup:
		fseek(idx, 0, SEEK_SET);
		new_index->entries_no = i;
		freeIndex(new_index);

		fprintf(stderr, "Not enough memory for index\n");

		return NULL;
}

void freeIndex(Index * idx) {
	int i;
	
	for ( i = 0; i < idx->entries_no; i++ ) {
		free(idx->entries[i].data);
	}
		
	free(idx->entries);
	free(idx);

	return;
}

bool dumpIndex(Index * idx, FILE * idx_file, enum IndexType type) {
	int i, tmp;

	/* Write the number of registries */
	fseek(idx_file, 0, SEEK_SET);
	fwrite(&idx->entries_no, sizeof(idx->entries_no), 1, idx_file);

	/* Write each registry */
	for ( i = 0; i < idx->entries_no; i++ ) {
		switch ( type ) {
			case ISBN:
				if ( fwrite(idx->entries[i].isbn, ISBN_SIZE, 1, idx_file) < 1 )
					return false;
				
				if ( fwrite(idx->entries[i].data, RRN_SIZE, 1, idx_file) < 1)
					return false;
				
				break;

			case YEAR:
				if ( fwrite(idx->entries[i].data, YEAR_SIZE, 1, idx_file) < 1 )
					return false;
				
				if ( fwrite(idx->entries[i].isbn, ISBN_SIZE, 1, idx_file) < 1 )
					return false;

				break;
			default:
				tmp = strlen(idx->entries[i].data);

				if ( fwrite(idx->entries[i].data, sizeof(char), tmp, idx_file) < tmp )
					return false;
				
				if ( fputc(DELIMITER, idx_file) == EOF )
					return false;
				
				if ( fwrite(idx->entries[i].isbn, sizeof(char), ISBN_SIZE, idx_file) < ISBN_SIZE )
					return false;

				break;
		}
	}

	fseek(idx_file, 0, SEEK_SET);

	return true;
}

int compareISBN(const void * e1, const void * e2) {
	return strncmp(((const IndexEntry *) e1)->isbn,
				   ((const IndexEntry *) e2)->isbn,
				   ISBN_SIZE);
}

int compareYear(const void * e1, const void * e2) {
	return strncmp(
		(char *) ((const IndexEntry *) e1)->data,
		(char *) ((const IndexEntry *) e2)->data,
		YEAR_SIZE);
}

int compareWords(const void * e1, const void * e2) {
	return strncasecmp(
		(char *) ((const IndexEntry *) e1)->data,
		(char *) ((const IndexEntry *) e2)->data,
		WORD_MAX);
}

int searchIndex(Index * idx, char * data, enum IndexType type) {
	IndexEntry target;
	int (* cmp) ();
	int ret;

	switch ( type ) {
		case ISBN:
			if (! validateISBN(data) ) {
				fprintf(stderr, "Tried to search invalid ISBN: %s\n", data);
				return -2;
			}

			/* Set up a target with the desired ISBN string for comparison */
			strncpy(target.isbn, data, ISBN_SIZE);
			cmp = compareISBN;

			break;
		
		case YEAR:
			target.data = malloc(YEAR_SIZE * sizeof(char));
			if (! target.data ) {
				fprintf(stderr, "Could not allocate space for target year\n");
				return -2;
			}

			if (! validateYear(data) ) {
				fprintf(stderr, "Tried to search invalid year: %s\n", data);
				free(target.data);
				return -2;
			}

			strncpy((char *) target.data, data, YEAR_SIZE);
			cmp = compareYear;

			break;
		
		default:
			target.data = malloc((WORD_MAX+1) * sizeof(char));
			if (! target.data ) {
				fprintf(stderr, "Could not allocate space for target word\n");
				return -2;
			}

			/* TODO - Validate data here */

			strncpy((char *) target.data, data, WORD_MAX);
			cmp = compareWords;

			break;
	}
			
	ret = binarySearch(idx->entries, idx->entries_no, ENTRY_SIZE, &target, cmp);

	if ( type != ISBN ) {
		free(target.data);
	}

	return ret;
}

bool sortIndexFile(FILE * index_file, enum IndexType type) {
	Index * idx;
	int (* cmp) ();

	switch ( type ) {
		case ISBN:
			cmp = compareISBN;
			break;
		case YEAR:
			cmp = compareYear;
			break;
		default:
			cmp = compareWords;
			break;
	}

	fseek(index_file, 0, SEEK_SET);

	idx = loadIndex(index_file, type);

	if (! idx ) {
		fprintf(stderr, "Could not load index file.\n");
		return false;
	}
	
	sortIndexEntries(idx, cmp);
	dumpIndex(idx, index_file, type);

	freeIndex(idx);

	return true;
}

char * IndexName(const char * catalogName, enum IndexType type) {
	char idxName[2049];
	char * ext;
	
	strncpy(idxName, catalogName, 2048);

	/*Changes extension from .dat to TYPE.idx*/
	
	ext = strstr(idxName, ".dat");	
	if (! ext) {
		ext = idxName + (strlen(idxName) - 1);
	}
	switch (type) {
		case ISBN:
			strcpy(ext, "ISBN.idx");
			break;
		case TITLE:
			strcpy(ext, "TITLE.idx");
			break;
		case AUTHOR:
			strcpy(ext, "AUTHOR.idx");
			break;
		case SUBJECT:
			strcpy(ext, "SUBJ.idx");
			break;
		case YEAR:
			strcpy(ext, "YEAR.idx");
			break;
		default:
			fprintf(stderr, "Passing invalid IndexType to IndexName function\n");
			return NULL;
	}
	return pathCat(INDEX_DIR, idxName);
}
