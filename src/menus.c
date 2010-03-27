#include "libs.h"

int mainMenu() {
    
    char aux;
    
    puts("==========================================================");
    puts("                     BOOK CATALOG                         ");
    puts("==========================================================");
    putchar('\n');
    puts("         First of all, choose what you want to do         ");
    putchar('\n');
    puts("+--------------------------------------------------------+");
    puts("| Option | What it does                                  |");
    puts("+========+===============================================+");
    puts("|   'c'  | Creates a new Catalog file, which contains    |");
    puts("|        | information about books.                      |");
    puts("+--------+-----------------------------------------------+");
    puts("|   'h'  | Gives you a hand with some manuals when       |");
    puts("|        | necessary.                                    |");
    puts("+--------+-----------------------------------------------+");
    puts("|   'a'  | Asks you some questions in order to add a new |");
    puts("|        | book to a specific Catalog.                   |");
    puts("+--------+-----------------------------------------------+");
    puts("|   'q'  | Searches for a specific book information.     |");
    puts("|        |                                               |");
    puts("+--------+-----------------------------------------------+");
    puts("|   'l'  | Gives you an HTML organized list of books     |");
    puts("|        | that belong to a catalog.                     |");
    puts("+--------+-----------------------------------------------+");
    puts("|   'm'  | Shows this menu                               |");
    puts("|        |                                               |");
    puts("+--------+-----------------------------------------------+");
    puts("|   'x'  | Exits Program.                                |");
    puts("|        |                                               |");
    puts("+--------+-----------------------------------------------+");
    putchar('\n');
    printf("Type your option:");
	
    INPUT_CLEAR;
    scanf("%c", &aux);
    return (int)(toupper(aux));
}


int retypeOptionMenu() {
  puts("Invalid option. Try again, or type (h) (without brackets) for help:");
  INPUT_CLEAR;
  return (int)toupper(getchar());
}

bool tryAgainMenu() {
   char c;
   printf("Would you like to try again? (y/n)");

   while(1) {   
      INPUT_CLEAR;
      c = getchar();
   
      switch (toupper(c)){
         case 'Y':
            return true;
         case 'N':
            exit (0);
      }
   }
}
   

void listprintMenu() {
  char catalogName[200];
  
  putchar('\n');
  puts("----------------------------------------------------------");
  puts("                 HTML Catalog Printing                    ");
  puts("----------------------------------------------------------");
  printf("Type the name of the Catalog you want to print:");
  
  INPUT_CLEAR;
  scanf("%[^\n]", catalogName);
  if(!printFile(catalogName))
    fprintf(stderr, "\nFile doesn't appear to exist.");
}

void queryMenu() {
  putchar('\n');
  puts("----------------------------------------------------------");
  puts("                          Query                           ");
  puts("----------------------------------------------------------");
  NOT_IMPLEMENTED;

  return;
}

void addBookMenu() {
  
  char auxstr[MAXSTRING_SIZE];
  char catalogName[MAXSTRING_SIZE];
  
  /*Array of structs containing questions and functions to be called later int next for()*/
  struct {
      char * info;
      bool (*func)(Book*, char*);
  }BookInfo[] = {
   {"Type the Title of the book:", setTitle},
   {"Type the Author of the book:", setAuthor},
   {"Type the Main Characters of the book:", setCharacter},
   {"Type the Summary of the book:", setSummary},
   {"Type the Subject of the book:", setSubject},
   {"Type the ISBN of the book:", setISBN},
   {"Type the book's Image Filename:", setImgFile},
   {"Type the Year of publication:", setYear}
   };
     
  putchar('\n');
  puts("----------------------------------------------------------");
  puts("                          Add Book                        ");
  puts("----------------------------------------------------------");
  printf("Type the name of the file where you want to append a book:");

   /*Book Allocation and Catalog*/
  Book* newBook = createBook();
  INPUT_CLEAR;
  scanf("%[^\n]", catalogName);
  FILE* Catalog = openFile(catalogName, "a+");
   
   for(int i=0 ; i<8; i++){
      printf("%s", BookInfo[i].info);
      INPUT_CLEAR;
      scanf("%[^\n]", auxstr);
      if( (!BookInfo[i].func(newBook, auxstr)) && tryAgainMenu() )
         i--;
   }
  
  /*Writes to catalog using macro included in books.h*/
  writeBook(newBook, Catalog);
  puts("----------------------------------------------------------");
  puts("               Book added successfully!                   ");
  puts("----------------------------------------------------------");
  
  /*Closes file and frees memory*/
  fclose(Catalog);
  freeBook(newBook);

  return;
}
void createCatalogMenu() {
  char auxstr[200];
  
  putchar('\n');
  puts("----------------------------------------------------------");
  puts("                      Create New Catalog                  ");
  puts("----------------------------------------------------------");
  printf("Type the name of the new catalog:");
  
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  if(createFile(auxstr, "a+"))
    printf(" '%s' succesfully created\n", auxstr);
  return;
}

void helpMenu() {
	printHelp();
  return;
}

int continueMenu() {
  
  printf("\nWould you like to continue (y/n) ?:");
  
  INPUT_CLEAR;
  switch (toupper(getchar())) {
    case 'Y' : return 'M';
    case 'N' : return 'X';
    default: return 'H';
  }

}
