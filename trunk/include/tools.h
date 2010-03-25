#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>

/* Definitions for file path validation */
#define ERROR 0
#define FILE_EXISTS 1
#define DIR_EXISTS 2

/* Validates a path to a regular file.
 * Receives: char * path - the file path
 * Returns: ERROR - if the path is invalid (some dir in path doesn't exist,
 * 		    path is not a regular file)
 * 	    FILE_EXISTS - if the path is valid (exists and is a regular file)
 * 	    DIR_EXISTS - if every dir in path exists, but the basename doesn't
 * errno is set accordingly when ERROR is returned.
 */
int validateFile(const char * path);
