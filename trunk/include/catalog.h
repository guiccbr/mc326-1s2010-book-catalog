/*Creates an HTML list of books within CatalogFile if CatalogFile exists,
* returning true. Otherwise, returns false.
*/
bool printList(const char* CatalogFile);

/*Searches for keys in catalog*/
bool query();

/*Shows interactiveInterface, for runtime user's input*/
void interactiveInterface();


/*Reads arguments from shell's program call*/
void nonInteractiveInterface(int argc, char* argv[]);

