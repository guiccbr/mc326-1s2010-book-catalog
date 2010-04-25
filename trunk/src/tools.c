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

bool expressionsReplacer (FILE * model, FILE * newfile, const int NUM_OF_KEYS, char * str1, ...) {

    char** key; char** subs;
	char readbuffer[BUF_LEN]; char writebuffer[BUF_LEN];
    char * pkey;
    int i=0;
	va_list args;
	
	/*Starts list of variable number of arguments*/
	va_start(args, str1);
	
	/*Allocating memory for keys and subs*/
	key = allocateSTRarray(NUM_OF_KEYS);
	subs = allocateSTRarray(NUM_OF_KEYS);
	
	/*Start Building Replacement Arrays*/
	do {
		/*Reads key*/
		if(!i)key[i] = str1;
		else key[i] = va_arg(args, char*);
		if( null(key[i]) || empty(key[i]) ) {
			printf("Tried to set NULL or Empty key");
			return false;
		}
		/*Reads sub*/
		subs[i] = va_arg(args, char*);
		if(null(subs[i])) {
			printf("Tried to set NULL subs. for key '%s'", key[i]);
			return false;
		}
		i++;
	}while(i < NUM_OF_KEYS);
	
	/*Test Print TODO: Decide to remove this function or not.*/
    for(i=0; i<NUM_OF_KEYS; i++) {
        printf("|%s|%s|\n",key[i], subs[i]);
	}

    /*Check Files*/
    if(null(model)) {
        INVALID_NULLFILE
    }if(null(newfile)) {
        INVALID_NULLFILE
    }
    
    /*Reads line of text to readbuffer, checking if EOF was reached*/
    while(fgets(readbuffer, BUF_LEN, model))	{
        /*Finds keys in readbuffer, writing to writebuffer*/
        char * pRBUFF = readbuffer;
        char * pWBUFF = writebuffer;
        for(i = 0; i < NUM_OF_KEYS; i++) {
            pkey = strstr(pRBUFF, key[i]);
            if(pkey) {
                /*Prints substring that comes before key*/
                snprintf(pWBUFF, KEY_POS + 1, "%s", pRBUFF);
                /*Prints substitute*/
                snprintf(pWBUFF + KEY_POS, strlen(subs[i]) + 1, "%s", subs[i]);
                /*Prints substring that comes after substitute - NOT NECESSARY - TAKE A LOOK*/
                snprintf(pWBUFF + KEY_POS + strlen(subs[i]), (BUF_LEN - (KEY_POS + strlen(subs[i]))) + 1, "%s", pRBUFF + KEY_POS + strlen(key[i]));
                /*Updates pointers*/
				pWBUFF += KEY_POS + strlen(subs[i]);
                pRBUFF += KEY_POS + strlen(key[i]);
				i--;
            }
        }
        /*Writes string on newfile*/
		if (empty(writebuffer))
			fprintf(newfile, "%s", readbuffer);
		else
			fprintf(newfile, "%s", writebuffer);
		
		/*Cleans buffers*/
		cleanstr(writebuffer);
		cleanstr(readbuffer);
    }
	/*Closes list of variable arguments*/
	va_end(args);
	
	/*Frees allocated memory*/
	free(key); free(subs);
	
	/*Returns true, if ok*/
    return true;
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

bool cleanstr(char * str) {
	
	if(str)
		str[0] = '\0';
	else
		printf("Tryed to clean NULL string");
		return false;
	return true;
}

char** allocateSTRarray(int n) {
	char** p = (char**)malloc(sizeof(char*)*n);
	if(!p) {
		printf("Allocating Problem");
		exit(1);
	}
	return p;
}
