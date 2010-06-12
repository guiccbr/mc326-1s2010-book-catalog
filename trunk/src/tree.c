#include "tree.h"

/* Rude testing Main Adds Years to Tree, and creates Files
 * I have not run it yet - Some lib problems. Gotta think.
 */
int main(void) {
	char year[10];	
	FILE * catalogRRNS;
	FILE * tree;
	int i = 0;

	printf("Testing Tree\n\n");
	printf("Adds years to tree:\n");

	/*Opens Index Files*/
	if(validateFile("idx/catalogRRNS.idx") != FILE_EXISTS) {
		catalogRRNS = fopen("idx/catalogRRNS.idx", "r+");
		createRrnsFile(catalogRRNS);
	}
	else {
		catalogRRNS = fopen("idx/catalogRRNS.idx", "r+");
	}

	if(validateFile("idx/catalogYearTree.idx") != FILE_EXISTS) {
		tree = fopen("idx/catalogYearTree.idx", "r+");
		createTreeFile(tree);
	}
	else {
		tree = fopen("idx/catalogYearTree.idx", "r+");
	}

	while(1) {
		printf("Year:");
		scanf("%s", year);
		
		if(year[0] == '0') {
			break;
		}
		/*ADDS BOOK TO CATALOG - Returns RRN of the new book
		 *in catalog. In this test RRN is i.*/

		/*ADDS Book TO catalogRRNS, for testing, let's suppose
		 *each Book has only an year as key.
		 */
		rrnsAddBook(catalogRRNS, i);
		
		/*Adds key year to tree*/
		treeInsertKey(year, YEAR, i, tree, catalogRRNS);
	}

	/*Prints Tree*/
	printf("printing tree");
	fclose(tree);
	fclose(catalogRRNS);
	return 0;
}

treeNode * split(treeNode * root, FILE * tree) {
	int last;
	int center;
	int i;
	treeNode * newNode;

	last = root->keysNo;
	center = last/2;
	
	/*Creates a new space for a Node in tree file
	 *or uses an available one.*/
	newNode = treeGetAvail(tree);
	
	/*Copies Keys and Pointers to the new Node*/
	for(i = (center + 1); i < last; i++) {
		newNode->key[i] = root->key[i];
		newNode->p[i] = root->p[i];
	}
	
	/*Sets end of nodes*/
	root->keysNo = center + 1;
	newNode->keysNo = last - root->keysNo;

	/*Checks leaf bit linking the new leaf to the list*/
	if( isLeaf(root) ) {
		newNode->nextLeaf = root->nextLeaf;
		root->nextLeaf = newNode->rrn;
	}
	
	/*Writes modification to file*/
	seekTreeRRN(tree, root->rrn);
	writeNode(root, tree);
	seekTreeRRN(tree, newNode->rrn);
	writeNode(newNode, tree);

	/*Returns new son*/
	return newNode;
}

treeNode * mallocNode() {
	treeNode * node;

	node = (treeNode*)malloc(sizeof(treeNode));	
	if(!node) {
		fprintf(stderr, "mallocNode: Memory Allocation Problem!\n Aborting...\n");
		exit(1);
	}
	node->keysNo = 0;
	return node;
}

bool treeInsertKey(char * key, enum IndexType keyType, int rrnsRRN, FILE * tree, FILE * rrnsFile) {
	int rootRRN;
	int lastKey;
	treeNode * newSon;
	treeNode * root;
	treeNode * newRoot;

	/*Reads root RRN*/
	rootRRN = getRootRRN(tree);
	seekTreeRRN(tree, rootRRN);
	/*Loads Root to memory*/
	root = mallocNode();
	getNextNode(root, tree);
	
	/*Adds key, checking if root has been splitted*/
	newSon = treeInsertKey_Rec(key, keyType, rrnsRRN, root, tree, rrnsFile);
	
	/*Creates a new root if newSon != NULL*/
	if(newSon) {
		lastKey = root->keysNo - 1;
		/*Gets a new space for root in file and updates pointer*/
		newRoot = treeGetAvail(tree); 
		newRoot->key[0] = root->key[lastKey];
		newRoot->p[0] = rootRRN;
		newRoot->p[1] = newSon->rrn;
		/*TODO: IMPLEMENT SETROOT*/
		setRootRRN(tree, newRoot->rrn);
		/*Writes new Root to file*/
		seekTreeRRN(tree, newRoot->rrn);
		writeNode(newRoot, tree);
		free(newRoot);
		free(newSon);
	}
	free(root);
	return true;
}
int getRootRRN(FILE * tree) {
	int rootRRN;
	
	/*Skips avail list*/
	fseek(tree, sizeof(int), SEEK_SET);
	
	/*Reads main Root RRN*/
	fread(&rootRRN, sizeof(int), 1, tree);
	return rootRRN;
}

treeNode * treeInsertKey_Rec(char * key, enum IndexType keyType, int rrnsRRN, treeNode * subRoot, FILE * tree, FILE * rrnsFile) {
	treeNode * newSon = NULL;
	int nextSonRRN;
	treeNode * nextSon;
	
	/*If node is a leaf, inserts key in it*/
	if( isLeaf(subRoot) ) {
		nodeInsertKey(key, rrnsRRN, keyType, subRoot, tree, rrnsFile);
		/*If leaf is full after inserting, splits it*/
		if(isNodeFull(subRoot)) {
			newSon = split(subRoot, tree);
		}
	}
	/*If node is not a leaf, finds the subtree where key must be inserted*/
	else {
		nextSonRRN = subRoot->p[nodeFindIndex(key, keyType, subRoot)];
		seekTreeRRN(tree, nextSonRRN);
		nextSon = mallocNode();
		getNextNode(nextSon, tree);
		newSon = treeInsertKey_Rec(key, keyType, rrnsRRN, nextSon, tree, rrnsFile);
		free(nextSon);
		/* If newSon != NULL, son node was splitted
		 * Needs to insert son in actual node
		 */
		if(newSon) {
			nodeInsertSon(newSon, subRoot, tree);
			/*If subRoot is full after inserting, splits it*/
			if(subRoot->keysNo == ORDER) {
				newSon = split(subRoot, tree);
			}
		}
	}
	
	return newSon;
}

void nodeInsertKey(char * key, int rrnsRRN, enum IndexType keyType, treeNode * node, FILE * tree, FILE * rrnsFILE) {
	int place;
	rrnNode * RRN;
	int i = 0;

	/*Finds place for next key*/
	place = nodeFindIndex(key, keyType, node);

	/*Moves keys and pointers if key doesn't exist*/
	if(strcmp(node->key[place], key) != 0) {
		for(i = (place + 1); (i <= node->keysNo); i++) {
			node->key[i] = node->key[i-1];
			node->p[i] = node->p[i-1];
		}
		/*Inserts Key*/
		node->key[place] = key;
		/*Points key to list of rrns in rrns FILE*/
		node->p[place] = rrnsRRN;
		/*writes Modification*/
		seekTreeRRN(tree, node->rrn);
		writeNode(node, tree);
	}
	/*If key already exists, add a new rrn to rrns list in file*/
	else {
		RRN = mallocRRN();
		seekRrnRRN(rrnsFILE, rrnsRRN);
		getNextRRN(RRN, rrnsFILE);
		RRN->next[keyType] = node->p[place];
		node->p[place] = rrnsRRN;
		/*Writes Modifications*/
		seekTreeRRN(tree, node->rrn);
		writeNode(node, tree);
		seekRrnRRN(rrnsFILE, rrnsRRN);
		writeRrn(RRN, rrnsFILE);
	}
}
int nodeFindIndex(char * key, enum IndexType keyType, treeNode * node) {
	int i = 0;
	int (* cmp)();

	switch (keyType) {
		case TITLE: case AUTHOR: case SUBJECT:
			cmp = compareWords;
			break;
		case YEAR:
			cmp = compareYear;
			break;
		case ISBN:
			cmp = compareISBN;
			break;
		default:
			fprintf(stderr, "nodeInsertKey: Invalid indexType!\n -Aborting...\n");
			exit(1);
	}	
	
	/*Searches for the first key[i] >= key and stops counting*/ 
	for(i = 0; (i < node->keysNo) && (cmp(key, node->key[i]) > 0); i++);
	
	return i;
}

treeNode * treeGetAvail(FILE * tree) {
	int first_avail;
	treeNode * AvailNode;

	rewind(tree);
	fread(&first_avail, sizeof(int), 1, tree);
	/*If there's no space available, increases file size*/
	if(first_avail == -1) {
		fseek(tree, 0, SEEK_END);
		/*TODO: Don't know if this line is going to work!*/
		first_avail = (ftell(tree) - TREE_HEADER)/sizeof(treeNode);
	}
	/*Loads Node*/
	seekTreeRRN(tree, first_avail);
	AvailNode = mallocNode();
	getNextNode(AvailNode, tree);	
	/* Checks next available of the list, for updating avail list
	 * If Node->rrn == -1 this node block is not being used
	 * Node->nextLeaf points to the next available Node Block.
	 */
	if(!isNodeAvailable(AvailNode)) {
		fprintf(stderr, "treeGetAvail(): Tryed to overwrite not available node!\n Aborting...\n");
	}
	AvailNode->rrn = first_avail;
	first_avail = AvailNode->nextLeaf;

	/*writes modification to avail list head*/
	rewind(tree);
	fwrite(&first_avail, sizeof(int), 1, tree);
	/*Returns*/
	return AvailNode;
}

void createRrnsFile(FILE * rrnsFile) {
	int i;
	int AvailListHead = -1;

	/*Prints Avail list head*/
	fwrite(&AvailListHead, sizeof(int), 1, rrnsFile);

	/*Inserts RRNS_HEADER*/
	for(i = ftell(rrnsFile); i<RRN_HEADER; i++) {
		fputc('0', rrnsFile);
	}

}

void createTreeFile(FILE * tree) {
	int i;
	int AvailListHead = -1;
	int MainRootRRN = 0;

	/*Writes Avail list head*/
	fwrite(&AvailListHead, sizeof(int), 1, tree);
	
	/*Writes main Root RRN*/
	fwrite(&MainRootRRN, sizeof(int), 1, tree);

	/*Inserts TREE_HEADER*/
	for(i = ftell(tree); i < TREE_HEADER; i++) {
		fputc('0', tree);
	}
}
void rrnsAddBook(FILE * rrnsFile, int bookRRN) {
	int first_available;
	int next_available;
	rrnNode * availRRN;
	int i;
	
	availRRN = mallocRRN();
	
	rewind(rrnsFile);
	/*Finds available block*/
	fread(&first_available, sizeof(int), 1, rrnsFile);

	if(first_available == -1) {
		fseek(rrnsFile, 0, SEEK_END);
	}
	else {
		seekRrnRRN(rrnsFile, first_available);
		getNextRRN(availRRN, rrnsFile);
		/*Gets next available to update avail list*/
		next_available = availRRN->next[ISBN];
		/*Updates avail list head*/
		rewind(rrnsFile);
		fwrite(&next_available, sizeof(int), 1, rrnsFile);
		/*Cleans new RRN*/
		for(i=0; i<5; availRRN->next[i] = 0, i++);
	}

	/*Sets new RRN*/
	availRRN->catalogRRN = bookRRN;
	
	/*Writes RRN block to file*/
	seekRrnRRN(rrnsFile, first_available);
	writeRrn(availRRN, rrnsFile);

	free(availRRN);
	return;
}

rrnNode * mallocRRN() {
	rrnNode * rrn;
	int i;
	
	/*Allocates Memory*/
	rrn = (rrnNode*)malloc(sizeof(rrnNode));	
	if(!rrn) {
		fprintf(stderr, "mallocRRN: Memory Allocation Problem!\n Aborting...\n");
		exit(1);
	}
	/*Cleans RRNS*/
	rrn->catalogRRN = 0;
	for(i=0; i<5; rrn->next[i] = 0, i++);
	
	/*Returns pointer*/	
	return rrn;
}
