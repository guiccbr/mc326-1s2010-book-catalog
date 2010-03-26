#include "tools.h"

/*Shows application implemented functions offering the possibility to run them.
* Returns option chosen by user.
*/
int mainMenu();

/*Gives the user chance to retype when, in runtime, an invalid signal is given to the application.
* Returns option chosen by user (y/n).
*/
int tryagainMenu();

/*Prints an HTML organized list of books in catalog
* NOT_IMPLEMENTED
*/
void listprintMenu();

/*Offers searching tools to the user
* NOT_IMPLEMENTED
*/
void queryMenu();

/*Interactively asks for information about a new book, appending it to a specified catalog*/
void addBookMenu();

/*Asks for path and name in order to create a new catalog*/
void createCatalogMenu();

/*Prints Help*/
void helpMenu();

/*Asks whether user wishes to continue running application or not.*/
int continueMenu();