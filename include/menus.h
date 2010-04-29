#include <stdbool.h>

#define MODEL_PATH "models"

/*Presents the main functions of the application, offering the possibility of
* running them.
* Returns the code of the required function to run.
*
*   |--Code--|----Function----|
*   |   'A'  | AddBook        |
*   |   'C'  | Create Catalog |
*   |   'H'  | Help           |
*   |   'M'  | Shows main menu|
*   |   'Q'  | Query          |
*   |   'L'  | Shows List     |
*
*/
int mainMenu();

/*Screen shows an organized HTML list of books from an user-chosen catalog
* NOT FULLY IMPLEMENTED
* Returns false on the occurrence of an exception searching for catalog.
* Returns true on success.
*/
bool listprintMenu();

/*Presents searching tools, offering user the possibility of running them
* NOT FULLY IMPLEMENTED
*/
bool queryMenu();

/*Asks for information (1-9) about a book inclusion:
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
*
* Returns true on success. False otherwise.
*/
bool addBookMenu();

/*Asks for path and name for a new catalog, creating it, if possible, and
* showing success message.
* If there's a file allocating problem, raises an error, shows the user, and
* returns false. Returns true on success.
*/
bool createCatalogMenu();

/*Prints help*/
void helpMenu();

/*Gives the option of comming back to MainMenu after succesfully finalyzing a
* function execution
*/
int continueMenu();

/*Offers the possibility of retyping when an invalid option is chosen in
* MainMenu*/
int retypeOptionMenu();

/*Gives user a chance to come back to normal execution in case of runtime
* exception*/
bool tryAgainMenu();
