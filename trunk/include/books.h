#include <ctype.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* String size definitions for data storage */
#define TITLE_SIZE 150
#define ISBN_SIZE 13
#define SUBJECT_SIZE 25
#define AUTHOR_SIZE 61
#define YEAR_SIZE 4
#define SUMMARY_SIZE 1686
#define CHARACTER_SIZE 100
#define IMGFILE_SIZE 9

/* Where to look for images. Will be concatenated with the image file name */
#define IMGPATH "img/"
/* The size of the above string, for vector allocation purposes */
#define IMGPATH_SIZE 4

typedef struct {
	char 	title[TITLE_SIZE],
		isbn[ISBN_SIZE],
		subject[SUBJECT_SIZE],
		author[AUTHOR_SIZE],
		year[YEAR_SIZE],
		summary[SUMMARY_SIZE],
		character[CHARACTER_SIZE],
		imgfile[IMGFILE_SIZE];
} Book;

/* Checks whether the first ISBN_SIZE chars in a string are digits
 * Receives: char * isbn - the string
 * Returns: true or false (as defined in stdbool.h) if isbn is or not valid
 */
bool validateISBN(char * isbn);

/* Checks whether the first YEAR_SIZE chars in a string are digits
 * Receives: char * year - the string
 * Returns: true or false (as defined in stdbool.h) if year is or not valid
 */
bool validateYear(char * year);

/* Checks whether a string is in the correct form of an image filename.
 * The string should be '\0'-terminated and contain no more than
 * IMGFILE_SIZE + 1 characters.
 * Receives: char * imgfile - the image file name.
 * Returns: true or false (as defined in stdbool.h) if imgfile is or not valid
*/
bool validateImgFile(char * imgfile);

/* Transforms an image file name for storage.
 * Receives: char * img - the image file name, in the form filename.png

 * img gets set to filenamepng.
*/
void transformImgEntry(char * img);

/* Creates a new empty book. Its fields should be filled with the corresponding
 * set* function.
 * Returns: A pointer to a new book, or NULL in case of errors
 */
Book * createBook(void);

/* Frees all allocated resources for a book
 * Receives: Book * book - a book that will be free'd
 */
void freeBook(Book * book);

/* Checks if a string consists of comma-separated substrings that conform to
 * validateAlNumBlank in tools.c.
 * Receives: char * caracters - the string
 * Returns: true or false (as defined in stdbool.h)
 * Note - This function modifies the input string
 */
bool validateCharacters(char * characters);

/* Sets the title of a book to a given '\0'-terminated string.
 * Checks for null string, buffer overflow and invalid characters
 * Receives: Book * book - the book object
 * 	     char * title - the title string
 * Returns: true on success or false otherwise (as defined in stdbool.h)
 */
bool setTitle(Book * book, char * title);

/* Sets the author of a book to a given '\0'-terminated string.
 * Checks for null string, buffer overflow and invalid characters
 * Receives: Book * book - the book object
 * 	     char * author - the author string
 */
bool setAuthor(Book * book, char * author);

/* Sets the year of a book to a '\0'-terminated string.
 * Checks for null string, wrong string size and invalid characters
 * Receives: Book * book - the book object
 * 	     char * year - the year string
 * Returns: true or false (as defined in stdbool.h) upon success or failure
 */
bool setYear(Book * book, char * year);

/* Sets the ISBN of a book to a '\0'-terminated string.
 * Checks for null string, wrong string size and invalid characters
 * Receives: Book * book - the book object
 * 	     char * isbn - the isbn string
 * Returns: true or false (as defined in stdbool.h) upon success or failure
 */
bool setISBN(Book * book, char * isbn);

/* Sets the subject of a book to a given '\0'-terminated string.
 * Checks for null string, buffer overflow and invalid characters
 * Receives: Book * book - the book object
 * 	     char * subject - the subject string
 */
bool setSubject(Book * book, char * subject);

/* Sets the summary of a book to a given '\0'-terminated string.
 * Checks for null string, buffer overflow and invalid characters
 * Receives: Book * book - the book object
 * 	     char * summary - the summary string
 */
bool setSummary(Book * book, char * summary);

/* Sets the character of a book to a given '\0'-terminated string.
 * Checks for null string, buffer overflow and invalid characters
 * Receives: Book * book - the book object
 * 	     char * character - the character string
 */
bool setCharacter(Book * book, char * character);

/* Sets the image file of a book to a given '\0'-terminated string.
 * Checks for null string and buffer overflow 
 * Receives: Book * book - the book object
 * 	     char * imgfile -the image file basename
 */
bool setImgFile(Book * book, char * imgfile);
