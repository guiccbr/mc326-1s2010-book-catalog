/*----------------------------------------------------------------------------------------*/
/*Presents the main functions of the application, offering the possibility of running them.
* Returns the code of the required function to run.
*
*	|------Code-----|----Function----|
*	|	    'A'		 | AddBook			|
*	|		 'C'		 | Create Catalog	|
*	|		 'H'		 | Help				|
*	|		 'M'		 | Shows main menu|
*	|		 'Q'		 | Query				|
*	|		 'L'		 | Shows List		|
*
*/
int mainMenu();
/*-----------------------------------------------------------------------------------------*/
/*Screen shows an organized HTML list of books from an user-chosen catalog
* NOT FULLY IMPLEMENTED
*/
void listprintMenu();
/*-----------------------------------------------------------------------------------------*/
/*Presents searching tools, offering user the possibility of running them
* NOT FULLY IMPLEMENTED
*/
void queryMenu();
/*-----------------------------------------------------------------------------------------*/
/*Asks for information (1-10) about a book inclusion:
*
* 1) Place (Catalog) of inclusion.
* 2) Title
* 3) Author
* 4) Characters
* 5) Summary
* 6) Subject
* 7) ISBN
* 8) Year of Publication
* 9) Image File
*/
void addBookMenu();
/*-----------------------------------------------------------------------------------------*/
/*Asks for path and name for a new catalog, creating it, if possible, and showing success message.
* If there's a file allocating problem, raises an error, shows the user, and completely kills app.
*/
void createCatalogMenu();
/*-----------------------------------------------------------------------------------------*/
/*Prints help*/
void helpMenu();
/*-----------------------------------------------------------------------------------------*/
/*Gives the option of comming back to MainMenu after succesfully finalyzing a function execution*/
int continueMenu();
/*-----------------------------------------------------------------------------------------*/
int tryagainMenu();
bool tryAgainMenu();

