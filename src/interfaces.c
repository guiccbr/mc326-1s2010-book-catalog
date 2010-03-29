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
  exit (0);
}

void nonInteractiveInterface(int argc, char* argv[]) {

    /*Precheck Parameters*/
    if(argc<19) {
        MISSING_ARGUMENTS;
        exit(1);
    }
    if(argc>19){
        DUPLICATED_PARAMETERS;
        exit(1);
    }

    /*Noninteractive Body*/
    int opt_index = 0; int opt; int pCheck;
    int exit_code = 0;
    const char* short_opt = "t:A:y:I:S:s:c:i:";
    const char* filename = optarg;
    struct option long_opt[] = {
        {"title", 1, 0, 't'},
        {"author", 1, 0, 'A'},
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
    
    if (! (catalog && newBook) ) {
    	    printf("Exiting\n");
    	    /* createBook failed */
	    if (catalog) fclose(catalog);
	    /* openFile failed */
	    if (newBook) freeBook(newBook);
	    exit(1);
    }

/*
    if(catalog == NULL) {
        FILEALLOCERROR;
        exit_code = 1;
    }
*/

    /*Checks parameters, and decides what to do*/
    for(pCheck = 0;( NEXT_OPT != -1) && !(exit_code); pCheck++) {
        switch (opt){
            case 't':
                if(long_opt[0].has_arg==1){
                    long_opt[0].has_arg=0;
                    if(!setTitle(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 'A':
                if(long_opt[1].has_arg==1){
                    long_opt[1].has_arg=0;
                    if(!setAuthor(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 'y':
                if(long_opt[2].has_arg==1){
                    long_opt[2].has_arg=0;
                    if(!setYear(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 'I':
                if(long_opt[3].has_arg==1){
                    long_opt[3].has_arg=0;
                    if(!setISBN(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 'S':
                if(long_opt[4].has_arg==1){
                    long_opt[4].has_arg=0;
                    if(!setSubject(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 's':
                if(long_opt[5].has_arg==1){
                    long_opt[5].has_arg=0;
                    if(!setSummary(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 'c':
                if(long_opt[6].has_arg==1){
                    long_opt[6].has_arg=0;
                    if(!setCharacter(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            case 'i':
                if(long_opt[7].has_arg==1){
                    long_opt[7].has_arg=0;
                    if(!setImgFile(newBook, optarg))
                        exit_code = 1;
                    break;
                }DUPLICATED_PARAMETERS;exit_code = 1;
            default:
                invalidParameter(opt);
                exit_code = 1;
                break;
        }
    }
    if( (! exit_code) && pCheck < 8) {
        MISSING_ARGUMENTS;exit_code = 1;
        exit_code = 1;
    }

    /*Writes to catalog using macro included in books.h*/
    if(exit_code == 0) {
        writeBook(newBook, catalog);
        puts("Book added successfully!");
    }

    /*Closes file and frees memory*/
    fclose(catalog);
    freeBook(newBook);

    exit(exit_code);
}
