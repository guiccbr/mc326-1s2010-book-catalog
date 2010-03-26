/* Definitions for file path validation */
#define ERROR 0
#define FILE_EXISTS 1
#define DIR_EXISTS 2

#define NOT_IMPLEMENTED fprintf(stderr, "This function has not been implemented yet. Wait for comming versions\n")
#define HELP_FILE "help.dat"
#define FILEALLOCERROR fprintf(stderr, "File or allocating problem.\n")
#define NEXT_OPT (opt = getopt_long(argc, argv, short_opt, long_opt, &opt_index))
#define CATNAME_SIZE 15
#define INPUT_CLEAR __fpurge(stdin)
#define MAXSTRING_SIZE 2000
#define DUPLICATED_PARAMETERS fprintf(stderr, "Too many arguments\n"); exit(1);
#define MISSING_ARGUMENTS fprintf(stderr, "Missing Arguments\n"); exit (1);

/* Validates a path to a regular file.
 * Receives: char * path - the file path
 * Returns: ERROR - if the path is invalid (some dir in path doesn't exist,
 * 		    path is not a regular file)
 * 	    FILE_EXISTS - if the path is valid (exists and is a regular file)
 * 	    DIR_EXISTS - if every dir in path exists, but the basename doesn't
 */
int validateFile(const char * path);

/*Prints Help from HelpFile if Helpfile exists, returning true. 
* Otherwise returns false.
*/
bool printFile(const char* File);

/*Prints a error message when dealing with invalid parameters*/
void invalidParameter(int opt);

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
