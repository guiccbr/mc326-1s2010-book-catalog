#include "catalog.h"

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
		struct stat info;
		char * basename;
		char opt;
		
		/* TODO - All this verification code should be in a separate function */

		if ( stat(filename, &info) == -1 ) {
			/* TODO - Any other errors we should catch? */
			switch (errno) {
				case EACCES: /* No search permissions */
				case EFAULT: /* Malformed path */
					printf("Unable to open file: %s\n", strerror(errno));
					break;
				
				case ENOENT: 
					/* Some part of the path doesn't exist. */
					
					basename = strrchr(filename, '/');

					if ( basename != NULL ) { /* Checking the dirs above the filename */
						*basename = '\0';
						
						if ( stat(filename, &info) == -1 || (! S_ISDIR(info.st_mode))) {
							/* Something's wrong with the path, nothing we can do */
							fprintf(stderr, "Invalid catalog path: %s\n", filename);
							return NULL;
						}

						/* Path exists, file doesn't. Create it. */
						*basename = '/';
					}	

					/* else: filename doesn't exist and is relative to the current dir */
			}
	
		} else { /* File already exists */

			/* Catalogs should be regular files */
			if (! S_ISREG(info.st_mode) ) {
				fprintf(stderr, "Invalid catalog: %s\n", filename); 
				return NULL;
			}

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
		}

	return fopen(filename, mode);
}

FILE * openFile(const char* filename, const char* mode) {
	FILE * file = fopen(filename, "r");
	char opt;
	if (file == NULL) {
		puts("File doesn't exist. Do you wish to create it? (y/n): ");
        INPUT_CLEAR;
		scanf("%c", &opt);
		if (toupper(opt) == 'N') {
			fprintf(stderr, "Unable to open catalog\n");
			return file;
		}
	}
	else fclose(file);
	return fopen(filename, mode);

}

void invalidParameter(int opt) {
	fprintf(stderr, "Ivalid Parameter %c\n", opt);
	return;
}
