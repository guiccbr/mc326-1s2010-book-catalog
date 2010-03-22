#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include "books.h"
#include <string.h>
#include <stdio_ext.h>

#define NOT_IMPLEMENTED fprintf(stderr, "This function has not been implemented yet. Wait for comming versions\n")
#define HELP_FILE "help.dat"
#define FILEALLOCERROR fprintf(stderr, "File or allocating problem.\n")
#define NEXT_OPT (opt = getopt_long(argc, argv, short_opt, long_opt, &opt_index))
#define CATNAME_SIZE 15
#define INPUT_CLEAR __fpurge(stdin)
#define MAXSTRING_SIZE 2000


/*Prints Help from HelpFile if Helpfile exists, returning true. 
* Otherwise returns false.
*/
bool printFile(const char* File);

/*Creates an HTML list of books within CatalogFile if CatalogFile exists,
* returning true. Otherwise, returns false.
*/
bool printList(const char* CatalogFile);

/*Searches for keys in catalog*/
bool query();

/*Shows interactiveInterface, for runtime user's input*/
void interactiveInterface();

/*Prints a error message when dealing with invalid parameters*/
void invalidParameter(int opt);

/*Reads arguments from shell's program call*/
void nonInteractiveInterface(int argc, char* argv[]);

/*Checks if the file whose filename is the string pointed to by filename exists.
* If it does, associates a stream with it. Otherwise, asks if the user wants
* to create a new file.
* Returns the pointer to the file stream-associated, or NULL, if no file was open.
*/
FILE * openFile(const char* filename, const char* mode);

/*Checks if the file whose filename is the string pointed to by filename exist.
* If it does, ask permission to subscribe it. Otherwise, creates a file named filename.
* Returns NULL either if there's a problem allocating file or if permission to subscribe is denied.
*/
FILE * createFile(const char* filename, const char* mode);

