#include "libs.h"

void interactiveInterface() {
  int opt = mainMenu();
   
  while(opt != 'X'){
    switch (opt) {
      case 'C':
        createCatalogMenu();
        opt = continueMenu();
        break;
      case 'H':
        helpMenu();
        opt = continueMenu();
        break;
      case 'A':
        addBookMenu();
        opt = continueMenu();
        break;
      case 'Q':
        queryMenu();
        opt = continueMenu();
        break;
      case 'L':
        listprintMenu();
        opt = continueMenu();
        break;
      case 'M':
        opt = mainMenu();
        break;
      default:
        opt = retypeOptionMenu();
        break;
    }
  }
  return;
}

void nonInteractiveInterface(int argc, char* argv[]) {
	int opt_index = 0; int opt; int pCheck;
	const char* short_opt = "t:a:y:I:S:s:c:i:";
	const char* filename = optarg;
	struct option long_opt[] = {
		{"title", 1, 0, 't'},
		{"author", 1, 0, 'a'},
		{"year", 1, 0, 'y'},
		{"isbn", 1, 0, 'I'},
		{"subject", 1, 0, 'S'},
		{"summary", 1, 0, 's'},
		{"characters", 1, 0, 'c'},
		{"image", 1, 0, 'i'},
		{0,0,0,0}
	};
	
	/*Tries to open catalog and create a new book, stopping execution if not possible*/
	FILE* catalog = openFile(filename, "a+");
	Book* newBook = createBook();
	if(catalog == NULL) {
      FILEALLOCERROR;
      exit(1);
    }
    
	/*Checks parameters, and decides what to do*/
	for(pCheck = 0;( NEXT_OPT != -1); pCheck++) {
		switch (opt){
			case 't':
            if(long_opt[0].has_arg==1){
               long_opt[0].has_arg=0;
				   if(!setTitle(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 'a':
            if(long_opt[1].has_arg==1){
               long_opt[1].has_arg=0;
				   if(!setAuthor(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 'y':
            if(long_opt[2].has_arg==1){
               long_opt[2].has_arg=0;
				   if(!setYear(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 'I':
            if(long_opt[3].has_arg==1){
               long_opt[3].has_arg=0;
				   if(!setISBN(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 'S':
            if(long_opt[4].has_arg==1){
               long_opt[4].has_arg=0;
				   if(!setSubject(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 's':
            if(long_opt[5].has_arg==1){
               long_opt[5].has_arg=0;
				   if(!setSummary(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 'c':
            if(long_opt[6].has_arg==1){
               long_opt[6].has_arg=0;
				   if(!setCharacter(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			case 'i':
            if(long_opt[7].has_arg==1){
               long_opt[7].has_arg=0;
				   if(!setImgFile(newBook, optarg))
					   exit(1);
               break;
            }DUPLICATED_PARAMETERS;
			default:
				invalidParameter(opt);
				break;
		}
	}
   if(pCheck < 8) MISSING_ARGUMENTS;

	/*Writes to catalog using macro included in books.h*/
    writeBook(newBook, catalog);
    puts("\nBook added successfully!");
    
    /*Closes file and frees memory*/
    fclose(catalog);
    freeBook(newBook);
	
	exit(0);
}



