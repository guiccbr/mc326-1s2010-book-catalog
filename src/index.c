#include "index.h"
#include "tools.h"

bool createIndex(const char * catalog_file, char * index_file, enum IndexType type) {
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
			switch (type) {
				case ISBN: /* ISBN indexes relate ISBNs to RRNS */
					fwrite(block[i].isbn, sizeof(char), ISBN_SIZE, index);
					fwrite(&count, sizeof(int), 1, index);
					break;
				case TITLE: /* All other indexes relate their field to an ISBN */
					fwrite(block[i].title, sizeof(char), TITLE_SIZE, index);
					break;
				case SUBJECT:
					fwrite(block[i].subject, sizeof(char), SUBJECT_SIZE, index);
					break;
				case AUTHOR:
					fwrite(block[i].author, sizeof(char), AUTHOR_SIZE, index);
					break;
				case YEAR:
					fwrite(block[i].year, sizeof(char), YEAR_SIZE, index);
					break;
			}

			if ( type != ISBN ) {
				fwrite(block[i].isbn, sizeof(char), ISBN_SIZE, index);
			}
		}

		read = readBlock(block, catalog);
	}

	/* Write the number of registries */
	fseek(index, 0, SEEK_SET);
	fwrite(&count, sizeof(int), 1, index);

	fclose(index); /* Make sure nothing gets lost */

	if ( (type == ISBN) || (type == YEAR) ) {
		/* Reopen file for sorting */
		if (! (index = accessFile(index_file, "r+b")) ) return false;

		sortIndexFile(index, type);

		fclose(index);
	}

	fclose(catalog);

	return true;
}

Index * loadIndex(FILE * idx, enum IndexType type) {
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
		switch (type) {
			case ISBN:
				new_index->entries[i].data = malloc(sizeof(int));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
				fread(new_index->entries[i].data, RRN_SIZE, 1, idx);
				break;
			
			case TITLE:
				new_index->entries[i].data = malloc(TITLE_SIZE * sizeof(char));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].data, TITLE_SIZE, 1, idx);
				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
				break;
			
			case SUBJECT:
				new_index->entries[i].data = malloc(SUBJECT_SIZE * sizeof(char));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].data, SUBJECT_SIZE, 1, idx);
				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
				break;
			
			case AUTHOR:
				new_index->entries[i].data = malloc(AUTHOR_SIZE * sizeof(char));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].data, AUTHOR_SIZE, 1, idx);
				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
				break;
			
			case YEAR:
				new_index->entries[i].data = malloc(YEAR_SIZE * sizeof(char));
				if (! new_index->entries[i].data ) goto error_cleanup;

				fread(new_index->entries[i].data, YEAR_SIZE, 1, idx);
				fread(new_index->entries[i].isbn, ISBN_SIZE, 1, idx);
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

bool sortIndexFile(FILE * index_file, enum IndexType type) {
	Index * idx;

	fseek(index_file, 0, SEEK_SET);

	idx = loadIndex(index_file, type);

	if (! idx ) {
		fprintf(stderr, "Could not load index file.\n");
		return false;
	}
	
	/*sortIndexEntries(idx, (type == ISBN) ? compareISBN : compareYear);*/
	/* dumpIndex(idx, index_file, type); */
	sortIndexEntries(idx, compareISBN);
	dumpISBNIndex(idx, index_file);

	freeIndex(idx);

	return true;
}

char * ISBNIndexName(const char * catalogName) {
	char idxName[2049];
	char * ext;
	
	strncpy(idxName, catalogName, 2048);

	/*Changes extension from .dat to ISBN.idx*/
	
	ext = strstr(idxName, ".dat");	
	if (! ext) {
		ext = idxName + (strlen(idxName) - 1);
	}
	
	strcpy(ext, "ISBN.idx");

	return pathCat(INDEX_DIR, idxName);
}	
