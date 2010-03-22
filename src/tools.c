#include <tools.h>

int validateFile(const char * path) {
	struct stat info;
	char * basename;
	
	if ( stat(path, &info) == -1 ) {
		
		switch (errno) {
			case ENOENT: 
				/* Some part of the path doesn't exist. */

				basename = strrchr(path, '/');

				if ( basename != NULL ) { /* Checking the dirs above the path */
					*basename = '\0';

					if ( stat(path, &info) == -1 || (! S_ISDIR(info.st_mode))) {
						return ERROR;
					}

					/* Path exists, file doesn't. */
					*basename = '/';
				}	

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
