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
			printf("'%s' doesn't exist. Do you wish to create it? (y/n): ", filename);
			INPUT_CLEAR;
			scanf("%c", &opt);

			if (toupper(opt) != 'Y') {
				return NULL;
			}

			break;

	}

	return accessFile(filename, mode);
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
	char readbuffer[BUF_LEN];
	char writebuffer[BUF_LEN];
	char * pkey;
	int i=0;
	int firstsubs;
	va_list args;
	char * pRBUFF;
	char * pWBUFF;
	char * firstkey;

	/*Starts list of variable number of arguments*/
	va_start(args, str1);

	/*Allocating memory for keys and subs*/
	key = allocateSTRarray(NUM_OF_KEYS);
	subs = allocateSTRarray(NUM_OF_KEYS);

	/*Start Building Replacement Arrays*/
	do {
		/*Reads keys*/
		if(!i)key[i] = str1;
		else key[i] = va_arg(args, char*);
		if( null(key[i]) || empty(key[i]) ) {
			fprintf(stderr, "Tried to set NULL or Empty key");
			free(key); free(subs); return false;
		}
		/*Reads subs*/
		subs[i] = va_arg(args, char*);
		if(null(subs[i])) {
			fprintf(stderr, "Tried to set NULL subs. for key '%s'", key[i]);
			free(key); free(subs); return false;
		}
		i++;
	}while(i < NUM_OF_KEYS);

	/*Check Files*/
	if(null(model)) {
		INVALID_NULLFILE
		free(key); free(subs); return false;
	}if(null(newfile)) {
		INVALID_NULLFILE
		free(key); free(subs); return false;
	}
	
	while(fgets(readbuffer, BUF_LEN, model)) {
		/*Finds keys in readbuffer, writing to writebuffer*/
		pRBUFF = readbuffer;
		pWBUFF = writebuffer;
		
		while (1) {
			firstkey = NULL;
			firstsubs = 0;
			
			for(i = 0; i < NUM_OF_KEYS; i++) {
				pkey = strstr(pRBUFF, key[i]);
				
				if ( (! firstkey) || (pkey && (pkey < firstkey)) ) {
					firstsubs = i;
					firstkey = pkey;
				}
			}

			if(firstkey) {
				/*Prints substring that comes before key*/
				snprintf(pWBUFF, firstkey - pRBUFF + 1, "%s", pRBUFF);
				/*Prints substitute*/
				snprintf(pWBUFF + (firstkey - pRBUFF), strlen(subs[firstsubs]) + 1, "%s", subs[firstsubs]);
				/*Prints substring that comes after substitute - NOT NECESSARY - TAKE A LOOK*/
				snprintf(pWBUFF + (firstkey - pRBUFF) + strlen(subs[firstsubs]), (BUF_LEN - (firstkey - pRBUFF + strlen(subs[firstsubs]))) + 1, "%s", firstkey + strlen(key[firstsubs]));
				/*Updates pointers*/
				pWBUFF += firstkey - pRBUFF + strlen(subs[firstsubs]);
				pRBUFF += firstkey - pRBUFF + strlen(key[firstsubs]);
			} else {
				break;
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
	else {
		fprintf(stderr, "Tried to clean empty string");
		return false;
	}
	return true;
}

char** allocateSTRarray(int n) {
	int i;
	char** p = (char**)malloc(sizeof(char*)*n);
	if(!p) {
		fprintf(stderr, "Allocating Problem");
		exit(1);
	}
	for(i=0; i<n; p[i] = NULL, i++);
	return p;
}

char * adqStr(char * str, int size) {
	char * temp_str;
	int i;

	for(i=(size-1); i>=0; i--) {
		if(str[i] != ' ') {
			if(!(temp_str = (char*)calloc((i+1), sizeof(char)))) {
				fprintf(stderr, "Memory allocation problem");
				return NULL;
			}
			break;
		}
	}

	for(; i>=0; i--)
		temp_str[i] = str[i];

	return temp_str;
}

char * pathCat (char * dir, char * filename) {
	char * result = (char *) malloc(2049 * sizeof(char));
	int dirlength = strlen(dir);

	strncpy(result, dir, 2049);

	if ( dirlength <= 2047 ) {
		result[dirlength] = '/';
		result[dirlength+1] = '\0';
		strncat(result, filename, 2049 - (dirlength + 2));
	}

	return result;
}

char * appendNULL(char * str, int size) {
	char * result = (char *) malloc((size+1)*sizeof(char));

	if ( result ) {
		memset(result, 0, size+1);
		strncpy(result, str, size);
	}

	return result;
}
