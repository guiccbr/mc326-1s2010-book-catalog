#include "catalog.h"
#include "menus.h"

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

int tryagainMenu() {
  puts("Invalid option. Try again, or type (h) (without brackets) for help:");
  INPUT_CLEAR;
  return (int)toupper(getchar());
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
  printf("Type the ISBN of the Book you want to find:");

  return;
}

void addBookMenu() {
  
  char auxstr[MAXSTRING_SIZE];
  char catalogName[MAXSTRING_SIZE];
  
  putchar('\n');
  puts("----------------------------------------------------------");
  puts("                          Add Book                        ");
  puts("----------------------------------------------------------");
  printf("Type the name of the file where you want to append a book:");

  Book* newBook = createBook();
  INPUT_CLEAR;
  scanf("%[^\n]", catalogName);
  FILE* Catalog = openFile(catalogName, "a+");
  

  printf("Type the Title of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setTitle(newBook, auxstr);

  
  printf("Type the Author of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setAuthor(newBook, auxstr);
  
  printf("Type Main Characters of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setCharacter(newBook, auxstr);
  
  printf("Type the Summary of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setSummary(newBook, auxstr);
  
  printf("Type the Subject of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setSubject(newBook, auxstr);
  
  printf("Type the ISBN of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setISBN(newBook, auxstr);
  
  printf("Type the book's image filename:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setImgFile(newBook, auxstr);
  
  printf("Type the Year of the book:");
  INPUT_CLEAR;
  scanf("%[^\n]", auxstr);
  setYear(newBook, auxstr);
  
  /*Writes to catalog using macro included in books.h*/
  writeBook(newBook, Catalog);
  puts("\nBook added successfully!");
  
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
  printFile(HELP_FILE);
  return;
}

int continueMenu(char* str) {
  
  printf("\nWould you like to %s (y/n) ?:", str);
  
  INPUT_CLEAR;
  switch (toupper(getchar())) {
    case 'Y' : return 'M';
    case 'N' : return 'X';
    default: return 'H';
  }

}

