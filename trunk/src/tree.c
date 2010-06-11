#include "tree.h"

treeNode * split(treeNode * root, FILE * tree) {
	int last;
	treeNode * newNode;

	last = root->keysNo;
	medium = last/2;

	newNode = mallocNode();
	
	/*Copies Keys and Pointers to the new Node*/
	for(i = (medium + 1); i < last; i++) {
		newNode->key[i] = root->key[i];
		newNode->p[i] = root->p[i];
	}
	
	/*Sets end of nodes*/
	root->keysNo = medium + 1;
	newNode->keysNo = last - keysNo;

	/*gets New Node RRN and checks leaf bit*/
	newNode->rrn = getAvail(tree);
	if( isLeaf(root) ) {
		newNode->nextLeaf = root->nextLeaf;
		root->nextLeaf = newNode->rrn;
	}
	
	/*Writes modification to file*/
	writeNode(root, tree);
	writeNode(newNode, tree);

	/*Returns new son*/
	return newNode;
}

treeNode * mallocNode() {
	treeNode node;

	node = malloc(sizeof(treeNode));	
	if(!node) {
		fprintf(stderr, "mallocNode: Allocating Problem!");
		exit (1);
	}
	node->keysNo = 0;
	return node;
}

bool treeInsertKey(char * key, enum indexType keyType, int bookRRN, int rrnRRN, FILE * tree, FILE * rrnsFile) {
	int rootRRN;
	int lastKey;
	treeNode newSon;
	treeNode root;
	treeNode newRoot;

	
	/*Gets root RRN and loads it to memory*/
	getRoot(tree, &rootRRN);
	root = getNodeRRN(rootRRN);
	
	/*Adds key, checking if root has been splitted*/
	newSon = treeInsertKey_Rec(key, root, tree);
	
	/*Creates a new root if newSon != NULL*/
	if(newSon) {
		lastKey = root->keysNo - 1;
		newRoot = allocNode();
		if(!newRoot) {
			free(root);
			free(newSon);
			return false;
		}
		newRoot->key[0] = root->key[LastKey];
		newRoot->p[0] = rootRRN;
		newRoot->p[1] = newSon->rrn;
		/*Gets a new space for root in file and updates pointer*/
		newRoot->rrn = getAvail(tree);
		setRootRRN(tree, newRoot->rrn);
		/*Writes new Root to file*/
		writeNode(newRoot, tree);
		free(newRoot);
		free(newSon);
	}
	free(root);
	return true;
}
int getRoot(FILE * tree) {
	int rootRRN;
	
	rewind(tree);
	fscanf(tree, "%d", &rootRRN);
	return rootRRN;
}

treeNode * treeInsertKey_Rec(char * key, enum IndexType keyType, int bookRRN, int rrnRRN, treeNode subRoot, FILE * tree) {
	treeNode * newSon = NULL;
	int nextSonRRN;
	treeNode * nextSon;
	
	
	if( isleaf(subRoot) ) {
		nodeInsertKey(key, subRoot, tree);
		/*If leaf is full after inserting, splits it*/
		if(subRoot->keysNo == ORDER) {
			newSon = split(subRoot, tree);
		}
	}
	else {
		nextSonRRN = subRoot->p[nodeFindIndex(key, keyType, subRoot)];
		/*TODO: Check if it's NULL because allocation is done here*/
		nextSon = getNodeRRN(nextSonRRN);
		newSon = treeInsertKey_Rec(key, nextSon, tree);
		free(nextSon);
		/* If newSon != NULL, son node was splitted
		 * Needs to insert son in actual node
		 */
		if(newSon) {
			nodeInsertSon(newSon, subroot, Tree);
			/*If subRoot is full after inserting, splits it*/
			if(subRoot->keysNo == ORDER) {
				newSon = split(subRoot, tree);
			}
		}
	}
	
	return newSon;
}

void nodeInsertKey(char * key, int bookRRN, int rrnRRN, enum indexType keyType, treeNode * node, FILE * tree, FILE * rrnsFILE) {
	int place;
	int rrnRRN;
	int i = 0, j = 0;
	
	/*Finds place for next key*/
	place = nodeFindIndex(node);

	/*Moves keys and pointers if key doesn't exist*/
	if(cmp(node->key[place]) != 0) {
		for(i = (place + 1); (i <= node->keysNo); i++) {
			node->key[i] = node->key[i-1];
			node->p[i] = node->p[i-1];
		}
		/*Points key to list of rrns in rrns FILE*/
		node->p[place] = rrnRRN;
			
		
	}
	/*If key already exists, add a new rrn to rrns list in file*/
	else {
		seekRRN(rrnsFILE, node->p[place]);
		
		
			
	/*Moves keys to accomplish insertion*/
	for(j = node->keysNo; j > i; j--) {
		node->key[j] = node->key[j-1];
		node->p[j] = node->p[j-1];
	}

	/*Inserts new key in it's correct position*/
	
	
