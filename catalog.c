#include "catalog.h"

bool printFile(const char* File) {
	FILE* f = fopen(File, "r");
	char c;
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
	FILE * file = fopen(filename, "r");
	char opt;
	if(file != NULL){
		puts("File already exists. Do you wish to overwrite it? (y/n)");
        INPUT_CLEAR;
		scanf("%c", &opt);
		if(toupper(opt) == 'N'){
			fprintf(stderr, "Unable to create new catalog\n");
			return NULL;
		}else{	
         fclose(file);
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
