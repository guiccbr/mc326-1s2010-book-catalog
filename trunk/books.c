#ifndef BOOKS
#include "books.h"
#endif

bool validateISBN(char * isbn) {
	short int i = 0;

	while ( i < ISBN_SIZE ) {
		if (! isdigit(isbn[i++]) ) return false;
	}

	return true;
}

bool validateYear(char * year) {
	short int i = 0;

	while ( i < YEAR_SIZE ) {
		if (! isdigit(year[i++]) ) return false;
	}
	
	return true;
}

bool validateAlNumBlank(char * str, int size) {
	short int i = 0;

	while ( i < size ) {
		if (! (isalnum(str[i]) || isblank(str[i]))) return false;
		i++;
	}
	
	return true;
}

Book * createBook(void) {
	Book * b = calloc(sizeof(Book), 1);

	if ( b == NULL ) {
		fprintf(stderr, "Unable to malloc new book\n");
	} 

	return b;
}

void freeBook(Book * book) {
	free(book);
	return;
}

bool setTitle(Book * book, char * title) {
	char err_msg[35+TITLE_SIZE] = "Tried to set invalid book title: ";
	char formatted_title[TITLE_SIZE];
	int length = strlen(title);

	if (! title ) {
		fprintf(stderr, "Tried to set NULL book title\n");
		return false;
	}
	
	if ( length > TITLE_SIZE ) {
		fprintf(stderr, "Title too big -- truncating: %s\n", title);
		length = TITLE_SIZE;
	}
	
	if (! validateAlNumBlank(title, length)) {
		strncat(err_msg, title, length);
		fprintf(stderr, "%s\n", err_msg);
		return false;
	}
 	
 	/* strlen(title) < TITLE_SIZE, title must be padded */
	if ( length != TITLE_SIZE ) {
	
		/* Filling the formatted title string with spaces for padding */
		memset(formatted_title, 32, TITLE_SIZE);
		
		/* Writing the title parameter to the beginning of the formatted title;
		 * formatted_title now contains the title padded with spaces to TITLE_SIZE
		 */
		memcpy(formatted_title, title, length);

		memcpy(book->title, formatted_title, TITLE_SIZE);
	
	} else { /* strlen(title) == TITLE_SIZE, no padding needed */
		memcpy(book->title, title, TITLE_SIZE);
	}

	return true;
}

bool setAuthor(Book * book, char * author) {
	char err_msg[36+AUTHOR_SIZE] = "Tried to set invalid book author: ";
	char formatted_author[AUTHOR_SIZE];
	int length = strlen(author);

	if (! author ) {
		fprintf(stderr, "Tried to set NULL book author\n");
		return false;
	}
	
	if ( length > AUTHOR_SIZE ) {
		fprintf(stderr, "Author too big -- truncating: %s\n", author);
		length = AUTHOR_SIZE;
	}
	
	if (! validateAlNumBlank(author, length)) {
		strncat(err_msg, author, AUTHOR_SIZE);
		fprintf(stderr, "%s\n", err_msg);
		return false;
	}

	/* strlen(author) < AUTHOR_SIZE, author must be padded */
	if ( length != AUTHOR_SIZE ) { 
	
		/* Filling the formatted author string with spaces for padding */
		memset(formatted_author, 32, AUTHOR_SIZE);
		
		/* Writing the title parameter to the beginning of the formatted author;
		 * formatted_author now contains the title padded with spaces to AUTHOR_SIZE
		 */
		memcpy(formatted_author, author, length);

		memcpy(book->author, formatted_author, AUTHOR_SIZE);
	
	} else { /* strlen(author) == AUTHOR_SIZE, no padding needed */
		memcpy(book->author, author, AUTHOR_SIZE);
	}

	return true;
}

bool setYear(Book * book, char * year) {

	if (! year ) {
		fprintf(stderr, "Tried to set NULL year.\n");
		return false;
	}
	
	if ( strlen(year) != YEAR_SIZE ) {
		fprintf(stderr, "Wrong year string size.\n");
		return false;
	}

	if (! validateYear(year) ) {
		fprintf(stderr, "Tried to set invalid year: %s\n", year);
		return false;
	}
		
	memcpy(book->year, year, YEAR_SIZE);

	return true;
}

bool setISBN(Book * book, char * isbn) {

	if (! isbn ) {
		fprintf(stderr, "Tried to set NULL ISBN.\n");
		return false;
	}
	
	if ( strlen(isbn) != ISBN_SIZE ) {
		fprintf(stderr, "Wrong ISBN string size.\n");
		return false;
	}

	if (! validateISBN(isbn) ) {
		fprintf(stderr, "Tried to set invalid ISBN: %s\n", isbn);
		return false;
	}
		
	memcpy(book->isbn, isbn, ISBN_SIZE);

	return true;
}

bool setSubject(Book * book, char * subject) {
	char err_msg[37+SUBJECT_SIZE] = "Tried to set invalid book subject: ";
	char formatted_subject[SUBJECT_SIZE];
	int length = strlen(subject);

	if (! subject ) {
		fprintf(stderr, "Tried to set NULL book subject\n");
		return false;
	}
	
	if ( length > SUBJECT_SIZE ) {
		fprintf(stderr, "Subject too big -- truncating: %s\n", subject);
		length = SUBJECT_SIZE;
	}
	
	if (! validateAlNumBlank(subject, length)) {
		strncat(err_msg, subject, length);
		fprintf(stderr, "%s\n", err_msg);
		return false;
	}
 	
 	/* strlen(subject) < SUBJECT_SIZE, subject must be padded */
	if ( length != SUBJECT_SIZE ) {
	
		/* Filling the formatted subject string with spaces for padding */
		memset(formatted_subject, 32, SUBJECT_SIZE);
		
		/* Writing the subject parameter to the beginning of the formatted subject;
		 * formatted_subject now contains the subject padded with spaces to SUBJECT_SIZE
		 */
		memcpy(formatted_subject, subject, length);

		memcpy(book->subject, formatted_subject, SUBJECT_SIZE);
	
	} else { /* strlen(subject) == SUBJECT_SIZE, no padding needed */
		memcpy(book->subject, subject, SUBJECT_SIZE);
	}

	return true;
}

bool setSummary(Book * book, char * summary) {
	char err_msg[37+SUMMARY_SIZE] = "Tried to set invalid book summary: ";
	char formatted_summary[SUMMARY_SIZE];
	int length = strlen(summary);

	if (! summary ) {
		fprintf(stderr, "Tried to set NULL book summary\n");
		return false;
	}
	
	if ( length > SUMMARY_SIZE ) {
		fprintf(stderr, "Summary too big -- truncating: %s\n", summary);
		length = SUMMARY_SIZE;
	}
	
	if (! validateAlNumBlank(summary, length)) {
		strncat(err_msg, summary, length);
		fprintf(stderr, "%s\n", err_msg);
		return false;
	}
 	
 	/* strlen(summary) < SUMMARY_SIZE, summary must be padded */
	if ( length != SUMMARY_SIZE ) {
	
		/* Filling the formatted summary string with spaces for padding */
		memset(formatted_summary, 32, SUMMARY_SIZE);
		
		/* Writing the summary parameter to the beginning of the formatted summary;
		 * formatted_summary now contains the summary padded with spaces to SUMMARY_SIZE
		 */
		memcpy(formatted_summary, summary, length);

		memcpy(book->summary, formatted_summary, SUMMARY_SIZE);
	
	} else { /* strlen(summary) == SUMMARY_SIZE, no padding needed */
		memcpy(book->summary, summary, SUMMARY_SIZE);
	}

	return true;
}

bool setCharacter(Book * book, char * character) {
	char err_msg[39+CHARACTER_SIZE] = "Tried to set invalid book character: ";
	char formatted_character[CHARACTER_SIZE];
	int length = strlen(character);

	if (! character ) {
		fprintf(stderr, "Tried to set NULL book character\n");
		return false;
	}
	
	if ( length > CHARACTER_SIZE ) {
		fprintf(stderr, "Character too big -- truncating: %s\n", character);
		length = CHARACTER_SIZE;
	}
	
	if (! validateAlNumBlank(character, length)) {
		strncat(err_msg, character, length);
		fprintf(stderr, "%s\n", err_msg);
		return false;
	}
 	
 	/* strlen(character) < CHARACTER_SIZE, character must be padded */
	if ( length != CHARACTER_SIZE ) {
	
		/* Filling the formatted character string with spaces for padding */
		memset(formatted_character, 32, CHARACTER_SIZE);
		
		/* Writing the character parameter to the beginning of the formatted character;
		 * formatted_character now contains the character padded with spaces to CHARACTER_SIZE
		 */
		memcpy(formatted_character, character, length);

		memcpy(book->character, formatted_character, CHARACTER_SIZE);
	
	} else { /* strlen(character) == CHARACTER_SIZE, no padding needed */
		memcpy(book->character, character, CHARACTER_SIZE);
	}

	return true;
}

bool setImgFile(Book * book, char * imgfile) {
	char err_msg[40+IMGFILE_SIZE] = "Tried to set invalid book image file: ";
	char formatted_imgfile[IMGFILE_SIZE];
	int length = strlen(imgfile);

	if (! imgfile ) {
		fprintf(stderr, "Tried to set NULL book image file\n");
		return false;
	}
	
	if ( length > IMGFILE_SIZE ) {
		fprintf(stderr, "Image size too big -- truncating: %s\n", imgfile);
		length = IMGFILE_SIZE;
	}
	
	if (! validateAlNumBlank(imgfile, length)) {
		strncat(err_msg, imgfile, length);
		fprintf(stderr, "%s\n", err_msg);
		return false;
	}
 	
 	/* strlen(imgfile) < IMGFILE_SIZE, imgfile must be padded */
	if ( length != IMGFILE_SIZE ) {
	
		/* Filling the formatted imgfile string with spaces for padding */
		memset(formatted_imgfile, 32, IMGFILE_SIZE);
		
		/* Writing the imgfile parameter to the beginning of the formatted imgfile;
		 * formatted_imgfile now contains the imgfile padded with spaces to IMGFILE_SIZE
		 */
		memcpy(formatted_imgfile, imgfile, length);

		memcpy(book->imgfile, formatted_imgfile, IMGFILE_SIZE);
	
	} else { /* strlen(imgfile) == IMGFILE_SIZE, no padding needed */
		memcpy(book->imgfile, imgfile, IMGFILE_SIZE);
	}

	return true;
}

/* Used only for testing -- prints a single quote followed by the 'size' chars
 * after ptr, then another single quote and a newline */
void _printchars(char * ptr, int size) {
	printf("'");
	while (size--)
		printf("%c", *ptr++);

	printf("'\n");
	return;
}

int main(void) {
	Book * b = createBook();
	FILE * out = fopen("catalogo_teste.dat", "rb");

	getNextBook(b, out);
	
	setTitle(b, "1984");
	setISBN(b, "1234567890123");
	setAuthor(b, "George Orwell");
	setYear(b, "1945");
	setCharacter(b, "Winston Smith");

	_printchars(b->title, TITLE_SIZE);
	_printchars(b->isbn, ISBN_SIZE);
	_printchars(b->author, AUTHOR_SIZE);
	_printchars(b->title, YEAR_SIZE);
	_printchars(b->character, CHARACTER_SIZE);

	/*writeBook(b, out);*/

	freeBook(b);

	return 0;
}
