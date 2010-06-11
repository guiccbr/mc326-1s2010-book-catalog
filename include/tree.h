#include <tools.h>

/* Tree Node definition*/
typedef struct node {
	char * key[ORDER];
	int pointer[ORDER];
	int nextLeaf;
	int keysNo;
	int rrn;
}treeNode;


/* rrn struct definition for recording in rrns file*/
typedef struct rrn {
	int rrn;
	int yearList;
	int authorList;
	int subjectList;
	int titleList;
	int isbn;
}

/* Macro that checks whether a node is a leaf or not,
 * returning true or false.
 */
#define isLeaf((node)) ((node)->nextLeaf != -2)

/* Divides a node in two, allocating memory for the
 * new node. In the first node, mantains the first
 * (ORDER/2 + 1) keys, keeping the rest in the new node.
 * Receives:	treeNode * root: Node to be splitted.
 *		FILE * tree: open tree file associated
 *			withe the node.
 * Returns a pointer (in memory) for the new allocated
 * node. Returns null in case of memory allocation prob.
 */
treeNode * split(treeNode * root, FILE * tree);

/* Allocs memory for a new node in memory.
 * Returns a pointer to a new allocated node.
 * Note: Displays error messagen in case of allocation
 * error. Sets the node->keysNO = 0.
 */
treeNode * mallocNode();

/* Inserts a new key in the B+ tree Index, recording any
 * modifications in tree file. Correctly splits root, if
 * necessary, creating a new root, and recording to file.
 * Receives:	char * key: Key to be inserted.
 *		enum IndexType keyType: Type of the key -
 *			ISBN, AUTHOR, TITLE, YEAR, (...)
 *		FILE * tree: Tree File.
 *		int bookRRN: RRN of the book associated to
 *			the key in book catalog.
 *		FILE * rrnsFILE: File of indexed rrns.
 *		int rrnRRN: RRN of the book rrn int rrnsFILE.
 * Returns true in sucess. False otherwise.
 */
bool treeInsertKey(char * key, enum indexType keyType, int bookRRN, int rrnRRN, FILE * tree, FILE * rrnsFile);

/* Gets the roots RRN of a tree FILE.
 * Receives:	FILE * tree: tree File.
 * Returns the RRN of main root of the tree.
 */
int getRoot(FILE * tree);

/* Recursive function that inserts a key in a B+ tree. Must
 * be called inside treeInsertKey().
 * Receives:	char * key: Key to be inserted.
 *		enum IndexType keyType: Type of the key -
 *			ISBN, AUTHOR, TITLE, YEAR, (...)
 *		FILE * tree: Tree File.
 *		int bookRRN: RRN of the book associated to
 *			the key in book catalog.
 *		int rrnRRN: RRN of the book rrn int rrnsFILE.
 *		treeNode * subRoot: The root of the tree where
 *			key must be inserted.
 *		FILE * rrnsFILE: File of indexed rrns.
 * Returns a pointer (memory) to a new son, that is the result
 * of a child split. Returns NULL if subRoot child has not been
 * splitted.
 */
treeNode * treeInsertKey_Rec(char * key, enum IndexType keyType, int bookRRN, int rrnRRN, treeNode * subRoot, FILE * tree, FILE * rrnsFile);

/* Inserts a key to a node's array of keys, finding the best
 * place to it, considering that the array must be sorted.
 * Receives:	char * key: Key to be inserted.
 *		enum IndexType keyType: Type of the key -
 *			ISBN, AUTHOR, TITLE, YEAR, (...)
 *		FILE * tree: Tree File.
 *		int bookRRN: RRN of the book associated to
 *			the key in book catalog.
 *		int rrnRRN: RRN of the book rrn int rrnsFILE.
 *		treeNode * node: The root of the tree where
 *			key must be inserted.
 *		FILE * rrnsFILE: File of indexed rrns.
 */
void nodeInsertKey(char * key, int bookRRN, int rrnRRN, enum indexType keyType, treeNode * node, FILE * tree);	
