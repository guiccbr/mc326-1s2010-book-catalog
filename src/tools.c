#include "tools.h"

int validateFile(const char * path) {
	struct stat info;
	char * dir = (char *) malloc(strlen(path) + 1);

	if ( dir == NULL ) {
		errno = ENOMEM;
		return ERROR;
	}

	strcpy(dir, path);
	
	if ( stat(path, &info) == -1 ) {
		
		switch (errno) {
			case ENOENT: 
				/* Some part of the path doesn't exist. Is it the path? */
				
				if ( stat(dirname(dir), &info) == -1 || (! S_ISDIR(info.st_mode))) {
					free(dir);
					return ERROR; /* Something's wrong with the path */
				}

				free(dir);
				
				/* Path exists, file doesn't. */
				return DIR_EXISTS;
				
			default:
				return ERROR;

		}
	
	} else {
		if (S_ISREG(info.st_mode)) return FILE_EXISTS;
	}
	
	/* Path exists but is not a regular file */
	errno = EFAULT;
	return ERROR;
}
