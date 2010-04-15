#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Definitions for file path validation */
#define ERROR 0
#define FILE_EXISTS 1
#define DIR_EXISTS 2

#define NOT_IMPLEMENTED fprintf(stderr, "This function has not been implemented yet. Wait for comming versions\n")
#define FILEALLOCERROR fprintf(stderr, "File or allocating problem.\n")
#define NEXT_OPT (opt = getopt_long(argc, argv, short_opt, long_opt, &opt_index))
#define CATNAME_SIZE 15
#define INPUT_CLEAR __fpurge(stdin)
#define MAXSTRING_SIZE 2000
#define DUPLICATED_PARAMETERS fprintf(stderr, "Too many arguments\n")
#define MISSING_ARGUMENTS fprintf(stderr, "Missing Arguments\n")
#define TYPE_YES_OR_NO printf("Invalid Option. Type y or n and press Enter: ")

extern const char * program_name;

/* Validates a path to a regular file.
 * Receives: char * path - the file path
 * Returns: ERROR - if the path is invalid (some dir in path doesn't exist,
 * path is not a regular file)
 * FILE_EXISTS - if the path is valid (exists and is a regular file)
 * DIR_EXISTS - if every dir in path exists, but the basename doesn't
 */
int validateFile(const char * path);

/* Checks if all chars from str to str+size are either alphanumeric or spaces
 * Receives: char * str - the string
 * 	     int size - the expected size of the string
 * Returns: true or false (as defined in stdbool.h)
 */
bool validateAlNumBlank(char * str, int size);

/* Prints usage information */
void printHelp(void);

/*Prints Help from HelpFile if Helpfile exists, returning true.
* Otherwise returns false.
*/
bool printFile(const char* File);

/*Prints a error message when dealing with invalid parameters*/
void invalidParameter(int opt);

/* Tries to open a file with a mode.
 * Receives: char * filename - the path of the file
 * 	     char * mode - the mode for fopen()
 * Returns: A FILE * associated with filename, or NULL on error.
 * Note - Displays an error message.
 */
FILE * accessFile(const char * filename, const char * mode);

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

/* Generic binary search of an element in a vector.
 * Receives: void * list - The beginning of the vector.
 *           int elements_no - The number of valid positions after list.
 *           size_t element_size - The size of each position.
 *           void * target - A pointer to the desired element.
 *           int (* cmp) () - A comparison function.
 * Returns:  The addres of the element if it was found, NULL otherwise.
 * Note: cmp(x,y) -> < 0, 0, > 0 if x < y, x == y, x > 0 respectively.
 */
void * binarySearch(void * list, int elements_no, size_t element_size, void * target, int (* cmp) ());
