#include "catalog.h"
#include "menus.h"
#include "tools.h"

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
        opt = tryagainMenu();
        break;
    }
  }
  return;
}

void nonInteractiveInterface(int argc, char* argv[]) {
    int opt_index = 0; int opt; 
    const char* short_opt = "t:a:y:I:S:s:c:i:";
    const char* filename = optarg;
    const struct option long_opt[] = {
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
    if((catalog == NULL) || (newBook == NULL)) {
      FILEALLOCERROR;
      exit(1);
    }
    
    /*Checks parameters, and decides what to do*/
    while( NEXT_OPT != -1) {
        switch (opt){
            case 't':
                if(!setTitle(newBook, optarg))
                    exit(1);
                break;
            case 'a':
                if(!setAuthor(newBook, optarg))
                    exit(1);
                break;
            case 'y':
                if(!setYear(newBook, optarg))
                    exit(1);
                break;
            case 'I':
                if(!setISBN(newBook, optarg))
                    exit(1);
                break;
            case 'S':
                if(!setSubject(newBook, optarg))
                    exit(1);
                break;
            case 's':
                if(!setSummary(newBook, optarg))
                    exit(1);
                break;
            case 'c':
                if(!setCharacter(newBook, optarg))
                    exit(1);
                break;
            case 'i':
                if(!setImgFile(newBook, optarg))
                    exit(1);
                break;
            default:
                invalidParameter(opt);
                break;
        }
    }
    /*Writes to catalog using macro included in books.h*/
    writeBook(newBook, catalog);
    puts("\nBook added successfully!");
    
    /*Closes file and frees memory*/
    fclose(catalog);
    freeBook(newBook);
    
    exit(0);
}