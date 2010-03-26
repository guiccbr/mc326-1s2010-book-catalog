#include "libs.h"

FILE * createFile(const char* filename, const char* mode) {
	char opt;

	switch ( validateFile(filename) ) {
		case ERROR:
			return NULL;

		case FILE_EXISTS:
			puts("File already exists. Do you wish to overwrite it? (y/n)");
			INPUT_CLEAR;
			scanf("%c", &opt);

			/* Do not overwrite unless explicitly allowed to do so */
			if(toupper(opt) != 'Y'){
				return NULL;
			}else{
				remove(filename);
			}
			
			break;
	}
	
	/* May still return NULL if the mode is unavailable */
	return fopen(filename, mode);
}

FILE * openFile(const char* filename, const char* mode) {
	char opt;

	switch ( validateFile(filename) ) {
		case ERROR:
			return NULL;

		case DIR_EXISTS:
			puts("File doesn't exist. Do you wish to create it? (y/n): ");
			INPUT_CLEAR;
			scanf("%c", &opt);
			
			if (toupper(opt) != 'Y') {
				return NULL;
			}

			break;

	}

	/* May still return NULL if the mode is unavailable */
	return fopen(filename, mode);
}

void invalidParameter(int opt) {
	fprintf(stderr, "Invalid Parameter %c\n", opt);
	return;
}

int validateFile(const char * path) {
	struct stat info;
	char * dir;

	if ( stat(path, &info) == -1 ) {
		
		switch (errno) {
			case ENOENT: 
				/* Some part of the path doesn't exist. Is it the path? */

				dir = (char *) malloc(strlen(path) + 1);
				strcpy(dir, path);

				if ( dir == NULL ) return ERROR;
				
				if ( stat(dirname(dir), &info) == -1 || (! S_ISDIR(info.st_mode))) {
					free(dir);
					return ERROR; /* Something's wrong with the path */
				}

				free(dir);
				
				/* Path exists, file doesn't. */
				return DIR_EXISTS;
				
			default:
				return ERROR;

		}
	
	} else {
		if (S_ISREG(info.st_mode)) return FILE_EXISTS;
	}
	
	/* Path exists but is not a regular file */
	return ERROR;
}

void printHelp() {
	/* FIXME - should print argv[0] instead of ./catalog */
	printf("Usage: ./catalog [OPTIONS] CATALOG\n"
		"Valid options are:\n"
		"-h --help Prints this help\n"
		"-i --interactive Interactive mode, options are passed at runtime\n"
		"-c --create Only create a new catalog\n"
		"-a --add Add an entry to a catalog\n"
		"-q --query Search for primary key\n"
		"-l FILE Outputs the entries in the catalog to a file in HTML format\n");

	return;

}

/* Testing */

/* Used only for testing -- prints a single quote followed by the 'size' chars
 * after ptr, then another single quote and a newline */
void _printchars(char * ptr, int size) {
	printf("'");
	while (size--)
		printf("%c", *ptr++);

	printf("'\n");
	return;
}

/*Used only for testing -- prints the binary file for comparison*/
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
