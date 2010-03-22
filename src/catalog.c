#include "catalog.h"

#ifndef TOOLD
#include "tools.h"
#endif

bool printFile(const char* File) {
	FILE* f = fopen(File, "r");
	int c;
	if (f){
		while((c = fgetc(f)) != EOF)
			putchar(c);
		fclose(f);
		return true;
	}
	return false;
}

bool query() {
	NOT_IMPLEMENTED;
	return true;
}

bool printList(const char* CatalogFile) {
	NOT_IMPLEMENTED;
	return true;
}

FILE * createFile(const char* filename, const char* mode) {
	char opt;

	switch ( validateFile(filename) ) {
		case ERROR:
			fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
			return NULL;

		case FILE_EXISTS:
			puts("File already exists. Do you wish to overwrite it? (y/n)");
			INPUT_CLEAR;
			scanf("%c", &opt);

			/* Do not overwrite unless explicitly allowed to do so */
			if(toupper(opt) != 'Y'){
				fprintf(stderr, "Unable to create new catalog\n");
				return NULL;
			}else{	
				remove(filename);
			}
			
			break;
	}
	
	/* May still return NULL if the mode is unavailable
	 * TODO - Should we check for permissions here? */
	return fopen(filename, mode);	
}

FILE * openFile(const char* filename, const char* mode) {
	char opt;

	switch ( validateFile(filename) ) {
		case ERROR:
			fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
			return NULL;
		
		case DIR_EXISTS:
			puts("File doesn't exist. Do you wish to create it? (y/n): ");
			INPUT_CLEAR;
			scanf("%c", &opt);
			
			if (toupper(opt) != 'Y') {
				fprintf(stderr, "Unable to open catalog\n");
				return NULL;
			}

			break;

	}

	/* May still return NULL if the mode is unavailable
	 * TODO - Should we check for permissions here? */
	return fopen(filename, mode);
}

void invalidParameter(int opt) {
	fprintf(stderr, "Ivalid Parameter %c\n", opt);
	return;
}
