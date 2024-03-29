#include "books.h"
#include "tools.h"

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

bool validateCharacters(char * characters) {
	char * token;

	token = strtok(characters, ",");

	while ( token ) {
		if (! validateAlNumBlank(token, strlen(token)) ) return false;
		token = strtok(NULL, ",");
	}

	return true;
}

void transformImgEntry(char * img) {
	char * extension = strrchr(img, '.');

	while ( *extension != '\0' ) {
		extension[0] = extension[1];
		extension++;
	}

	return;
}

bool validateImgFile(char * imgfile) {
	char image_path[IMGFILE_SIZE + IMGPATH_SIZE + 1] = IMGPATH;
	char * extension = strrchr(imgfile, '.');

	if (! extension ) {
		fprintf(stderr, "Missing extension for image file.\n");
		return false;
	}

	if ( strncmp(extension + 1, "png", 3) ) {
		fprintf(stderr, "Invalid extension for image file.\n");
		return false;
	}

	strncat(image_path, imgfile, IMGFILE_SIZE + 1);
	if ( validateFile(image_path) != FILE_EXISTS ) {
		fprintf(stderr, "Warning: image file doesn't exist: %s.\n", image_path);
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
		fprintf(stderr, "Tried to set invalid book title: %s\n", title);
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
		fprintf(stderr, "Tried to set invalid book author: %s\n", author);
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
		fprintf(stderr, "Tried to set invalid book subject: %s\n", subject);
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

	/* Summary should be more permissive */
	/*
	if (! validateAlNumBlank(summary, length)) {
   		fprintf(stderr, "Tried to set invalid book summary:\n%s\n", summary);
   		return false;
   	}*/

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
	char formatted_character[CHARACTER_SIZE + 1];
	int length = strlen(character);

	if (! character ) {
		fprintf(stderr, "Tried to set NULL book characters\n");
		return false;
	}

	if ( length > CHARACTER_SIZE ) {
		fprintf(stderr, "Characters string too big -- truncating: %s\n", character);
		length = CHARACTER_SIZE;
	}

	memset(formatted_character, 0, CHARACTER_SIZE + 1);

	/* validateCharacters modifies its input string, using a copy */
	strncpy(formatted_character, character, length);

	if (! validateCharacters(formatted_character)) {
		fprintf(stderr, "Tried to set invalid book characters: %s\n", character);
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
	char formatted_imgfile[IMGFILE_SIZE];
	int length = strlen(imgfile);

	if (! imgfile ) {
		fprintf(stderr, "Tried to set NULL book image file\n");
		return false;
	}

	/* IMGILE_SIZE characters plus the extension separator '.' */
	if ( length > IMGFILE_SIZE + 1) {
		fprintf(stderr, "Image name size too big: %s\n", imgfile);
		return false;
	}

	if (! validateImgFile(imgfile) ) {
		return false;
	}

	transformImgEntry(imgfile);

	length--;

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

void printBookInfo(Book * book) {
	
	/*Note that TITLE is the largest field that will be printed*/
	/*If a larger one will be printed, the array info must be allocated
	differently*/
	char info[3][TITLE_SIZE + 1];
	
	if( (!book) ||
	(!book->title) ||
	(!book->year) ||
	(!book->isbn) ||
	(!book->subject) ||
	(!book->author)
	){ fprintf(stderr, "printBookInfo: NULL BOOK!\n"); return; }

	/*Adequates Strings*/
	adqStr(book->isbn, info[0], (ISBN_SIZE));
	adqStr(book->title, info[1], (TITLE_SIZE));
	adqStr(book->subject, info[2], (SUBJECT_SIZE));
	
	/*Prints Info*/
	printf("ISBN: "); printnf(info[0], ISBN_SIZE); printf(" | ");
	printf("TITLE: "); printnf(info[1], 15); printf(" | ");
	printf("SUBJECT: "); printnf(info[2], 15); printf("\n");
	
	return;	
}	
