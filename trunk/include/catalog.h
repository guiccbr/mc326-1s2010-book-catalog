#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include "books.h"
#include <string.h>

/*Definition for maximum catalog name string lenght*/
#define CATNAME_SIZE 15

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

/*Reads arguments from shell's program call*/
void nonInteractiveInterface(int argc, char* argv[]);

/*Checks if the file whose filename is the string pointed to by filename exist.
* If it does, ask permission to subscribe it. Otherwise, creates a file named filename.
* Returns NULL either if there's a problem allocating file or if permission to subscribe is denied.
*/
FILE * createFile(const char* filename, const char* mode);

