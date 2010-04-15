#include "tools.h"

FILE * accessFile(const char * filename, const char * mode) {
	FILE * f;
	
	f = fopen(filename, mode);
	
	if (! f ) {
		fprintf(stderr, "Couldn't open file %s with mode %s\n", filename, mode);
	}

	return f;
}

FILE * createFile(const char* filename, const char* mode) {
	char opt;

	switch(validateFile(filename)) {
		case ERROR:
			return NULL;
		case FILE_EXISTS:
			printf("File already exists. Do you wish to overwrite it (y/n)? ");
			while(1) {
				INPUT_CLEAR;
				scanf("%c", &opt);
				switch(toupper(opt)) {
					case 'Y':
						remove(filename);
						return fopen(filename, mode);
					case 'N':
						return NULL;
					default:
						TYPE_YES_OR_NO;
				}
			}
		default:
			return accessFile(filename, mode);
	}
}

FILE * openFile(const char* filename, const char* mode) {
	char opt;

	switch ( validateFile(filename) ) {
		case ERROR:
			fprintf(stderr,"Invalid filename\n");
			return NULL;

		case DIR_EXISTS:
			printf("File doesn't exist. Do you wish to create it? (y/n): ");
			INPUT_CLEAR;
			scanf("%c", &opt);

			if (toupper(opt) != 'Y') {
				return NULL;
			}

			break;

	}

	return accessFile(filename, mode);
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

bool validateAlNumBlank(char * str, int size) {
	short int i = 0;

	while ( i < size ) {
		if (! (isalnum(str[i]) || isblank(str[i]))) return false;
		i++;
	}
	
	return true;
}

void printHelp() {
	printf("Usage: %s [OPTIONS] CATALOG\n"
			"Valid options are:\n"
			"-h --help Prints this help\n"
			"-i --interactive Interactive mode, options are passed at runtime\n"
			"-c --create Only create a new catalog\n"
			"-a --add Add an entry to a catalog\n"
			"-q --query Search for primary key\n"
			"-l FILE Outputs the entries in the catalog to a file in HTML format\n", program_name);

	return;
}

void * binarySearch(void * list, int elements_no, size_t element_size, void * target, int (* cmp) ()) {
	int start = 0, end = elements_no;
	int candidate, res;

	while ( (end <= elements_no) && (start <= end) ) {
		candidate = ( end - start ) / 2;

		res = cmp(target, (((char *) list) + (start + candidate) * element_size));

		if (! res ) return (((char *) list) + (start + candidate) * element_size);

		if (res < 0) /* target < candidate */
			end = start + candidate - 1;
		else /* target > candidate */
			start = start + candidate + 1;
	}
	
	return NULL;
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
