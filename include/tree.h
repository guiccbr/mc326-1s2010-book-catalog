#include <tools.h>

/* Tree Node definition:
 * If Node->rrn == -1 this rrn block is not being used
 * and Node->nextLeaf points to the next available Node Block.
 * If Node->rrn != -1, this block is being used and
 * If Node->nextLeaf == -2, this block is not a leaf.
 * If Node->nextLeaf == -1, this block is a leaf and end of list.
 * If Node->nextLeaf >=0, this block is a leaf and is not the
 * end of the list.
 */
typedef struct node {
	char * key[ORDER];
	int pointer[ORDER];
	int nextLeaf;
	int keysNo;
	int rrn;
}treeNode;


/* rrn struct definition for recording in rrns file.
 * Must be used with enumIndexType.
 * next[AUTHOR] -> points to next author.
 * next[TITLE] -> points to next title.
 * next[YEAR] -> points to next year.
 * next[SUBJECT] -> points to next subject.
 * next[ISBN] must not exist. ISBN is a primary key.
 */
typedef struct rrn {
	int next[5];
}rrnNode

/* Macro that checks whether a node is a leaf or not,
 * returning true or false.
 */
#define isLeaf((node)) ((node)->nextLeaf != -2)

/* Macro that checks if a node is available to be used in file.
 * Returns true or false.
 */
#define isNodeAvailable((node)) ((node)->rrn == -1)

/* Retrieves the next node in the tree file
 * Receives:	treeNode * node: A treeNode pointer 
 *		(pointing to enough memory) that will store the data read.
 *		FILE * tree: The tree file.
 */
#define getNextNode((node), (tree)) fread((node), sizeof(treeNode), 1, (tree))

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
 *		FILE * rrnsFILE: File of indexed rrns.
 *		int rrnsRRN: RRN of the book rrn int rrnsFILE.
 * Returns true in sucess. False otherwise.
 */
bool treeInsertKey(char * key, enum indexType keyType, int rrnsRRN, FILE * tree, FILE * rrnsFile);

/* Gets the roots RRN of a tree FILE.
 * Receives:	FILE * tree: tree File.
 * Returns the RRN of main root of the tree.
 */
int getRootRRN(FILE * tree);

/* Recursive function that inserts a key in a B+ tree. Must
 * be called inside treeInsertKey().
 * Receives:	char * key: Key to be inserted.
 *		enum IndexType keyType: Type of the key -
 *			ISBN, AUTHOR, TITLE, YEAR, (...)
 *		FILE * tree: Tree File.
 *		int rrnsRRN: RRN of the book rrn int rrnsFILE.
 *		treeNode * subRoot: The root of the tree where
 *			key must be inserted.
 *		FILE * rrnsFILE: File of indexed rrns.
 * Returns a pointer (memory) to a new son, that is the result
 * of a child split. Returns NULL if subRoot child has not been
 * splitted.
 */
treeNode * treeInsertKey_Rec(char * key, enum IndexType keyType, int rrnsRRN, treeNode * subRoot, FILE * tree, FILE * rrnsFile);

/* Inserts a key to a node's array of keys, finding the best
 * place to it, considering that the array must be sorted.
 * Receives:	char * key: Key to be inserted.
 *		enum IndexType keyType: Type of the key -
 *			ISBN, AUTHOR, TITLE, YEAR, (...)
 *		FILE * tree: Tree File.
 *		int rrnsRRN: RRN of the book rrn int rrnsFILE.
 *		treeNode * node: The root of the tree where
 *			key must be inserted.
 *		FILE * rrnsFILE: File of indexed rrns.
 */
void nodeInsertKey(char * key, int rrnsRRN, enum indexType keyType, treeNode * node, FILE * tree);

/* Finds a place for the key in array of keys in node.
 * Receives:	char * key: Key to be inserted.
 *		enum IndexType keyType: Type of the key.
 *		treeNode * node: node where key will be inserted.
 * Returns the first index of the array of keys that is bigger or equal 
 * (alfabetically) to key.
 */
int nodeFindIndex(char * key, enum IndexType keyType, treeNode * node);

