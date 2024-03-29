#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdarg.h>
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

/* Definitions for expressions replacement */
#define EOS '\0'
#define BUF_LEN 4096
#define INVALID_NULLFILE fprintf(stderr, "Error:Null file\n");

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

/* Prints Help from HelpFile if Helpfile exists, returning true.
 * Otherwise returns false.
 */
bool printFile(const char* File);

/* Prints a error message when dealing with invalid parameters*/
void invalidParameter(int opt);

/* Tries to open a file with a mode.
 * Receives: char * filename - the path of the file
 * 	     char * mode - the mode for fopen()
 * Returns: A FILE * associated with filename, or NULL on error.
 * Note - Displays an error message.
 */
FILE * accessFile(const char * filename, const char * mode);

/* Checks if the file whose filename is the string pointed to by filename exists.
 * If it does, associates a stream with it. Otherwise, asks if the user wants
 * to create a new file.
 * Returns the pointer to the file stream-associated, or NULL, if no file was open.
 */
FILE * openFile(const char* filename, const char* mode);

/* Checks if the file whose filename is the string pointed to by filename exist.
* If it does, ask permission to subscribe it. Otherwise, creates a file named filename.
* Returns NULL either if there's a problem allocating file or if permission to subscribe is denied.
*/
FILE * createFile(const char* filename, const char* mode);

/* Generic binary search of an element in a vector.
 * Receives: void * list - The beginning of the vector.
 *           int elements_no - The number of valid positions after list.
 *           size_t element_size - The size of each position.
 *           void * target static- A pointer to the desired element.
 *           int (* cmp) () - A comparison function.
 * Returns:  The index of the first element found in list, or -1.
 * Note: cmp(x,y) -> < 0, 0, > 0 if x < y, x == y, x > 0 respectively.
 */
int binarySearch(void * list, int elements_no, size_t element_size, void * target, int (* cmp) ());

/* Gereric replacement of special strings (keys) for substitutes (subs).
 * Receives: FILE * model - File that contains key strings to be replaced.
 *		     FILE * newfile - Destination File with replaced keys of model.
 *			 const int NUM_OF_KEYS - Number of keys (strings that are going to be replaced).
 *			 char * str1, str2, ... - Starting from i=0, str[i+1] = subs for key str[i].
 * Returns:  True if replacement was successful. False, otherwise.
 * Note - Displays error messages
 */
bool expressionsReplacer (FILE * model, FILE * newfile, const int NUM_OF_KEYS, char * str1, ...);

/* 'Cleans' given non-NULL str setting str[0] = '\0'.
 * Returns True in sucess. False otherwise.
 */
bool cleanstr(char * str);

/* Allocates memory for an array of n strings.
 * Returns pointer for allocated memory.
 */
char** allocateSTRarray(int n);

/* Adequates string, stripping boundaries spaces, and concatenating termination character
 * Receives:	char * str - String to be adequated.
 					char * res - Resulting string. Must be pre-allocated, and must have enough space.
 * 				int size - Size of str.
 * Returns : 	char * - New adequated string.
 * Note : Notice that in file, termination characters are not stored. So, strings read from file to Book have to be adequated.
 * Usually, if the max size of the str is MAX, the max size of res must be (MAX + 1).
 */
char * adqStr(char * str, char * res, int size);

/* Safely joins a '\0'-terminated path name and a '\0'-terminated filename.
 * Receives: char * path - the base path
 * 	     char * filename - the final part of the resulting string
 * Returns: The joined string.
 * Note: The returned string should be free()'d.
 */
char * pathCat(char * path, char * filename);

/* Copies a string, appending a null character to it
 * Receives: char * str - a not '\0'-terminated string
 * 	     int size - the size of str
 * Returns: The resulting string, or NULL in case of error.
 * Note: The returned string must be free()'d
 */
char * appendNULL(char * str, int size);

/* Macro that checks if a pointer is NULL
 * Receives:	void * p - Pointer of any type.
 * Returns true if p is NULL. False otherwise.
 */
#define null(p) ((p) == NULL)

/* Macro that checks if a string is empty (str[0] == '\0').
 * Receives:	char * str - String to be analized.
 * Returns True if the is the first element of str is the terminating charater '\0'. Returns false otherwise.
 */
#define empty(str) ((str)[0] == '\0')

/* Macro that prints error message when an invalid parameter is given by user
 * Receives:	char opt - Invalid Parameter
 * Displays error message, and the invalid parameter.
 */
#define invalidParameter(opt) fprintf(stderr, "Invalid Parameter %c\n", opt);

/* Copies the intersection of two int arrays into an answer int array
 * Receives:	int * array1 - Array to be intersected with array2. Must contain a termination_value.
 *		 			int * array2 - Array to be intersected with array1. Must contain a termination_value.
 *					int * ans - Output array. Where the values of intersection will be copied to.
 *					int termination_value - Value that defines the end of an array of ints. Just like '\0' for strings.
 * Returns the number of elements in ans after intersectioning. -1 in error.
 * Note: Any kind of array passed will be treated as an array of ints.
 * Breaks if array1 and/or array2 do not contain the termination value.
 * Breaks if ans doesn't point to enough space. It means that ans must be at least as long as one of the intersecting arrays.
 * There's no problem in passing array1 or array2 as the output array.
 */
int intersection(int * array1, int * array2, int * ans, int termination_value);

int compareInts(const void * x, const void * y);

char * check_str(char * str);

bool yesOrNoMenu(const char * str);

/* Prints a fixed lenght field with first characters as string chars, and last characters as spaces ' '.
 * Receives: 	char * string - The string.
 *     		int n - field size (number of characters).
 * Note: Prints error message if string is null.
 */
void printnf(char * string, int n);




