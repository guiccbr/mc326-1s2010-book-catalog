#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdio_ext.h>

/* Definitions for file path validation */
#define ERROR 0
#define FILE_EXISTS 1
#define DIR_EXISTS 2

/* Extra tools definitions*/
#define NOT_IMPLEMENTED fprintf(stderr, "This function has not been implemented yet. Wait for comming versions\n")
#define NEXT_OPT (opt = getopt_long(argc, argv, short_opt, long_opt, &opt_index))
#define FILEALLOCERROR fprintf(stderr, "File or allocating problem.\n")
#define INPUT_CLEAR __fpurge(stdin)
#define MAXSTRING_SIZE 2000
#define HELP_FILE "help.dat"

/* Validates a path to a regular file.
 * Receives: char * path - the file path
 * Returns: ERROR - if the path is invalid (some dir in path doesn't exist,
 * 		    path is not a regular file)
 * 	    FILE_EXISTS - if the path is valid (exists and is a regular file)
 * 	    DIR_EXISTS - if every dir in path exists, but the basename doesn't
 * errno is set accordingly when ERROR is returned.
 */
int validateFile(const char * path);

/*Checks if the file whose filename is the string pointed to by filename exists.
* If it does, associates a stream with it. Otherwise, asks if the user wants
* to create a new file.
* Returns the pointer to the file stream-associated, or NULL, if no file was open.
*/
FILE * openFile(const char* filename, const char* mode);

/*Prints a error message when dealing with invalid parameters*/
void invalidParameter(int opt);
