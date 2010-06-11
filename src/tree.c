#include "tree.h"

treeNode * split(treeNode * root, FILE * tree) {
	int last;
	int center;
	treeNode * newNode;

	last = root->keysNo;
	center = last/2;

	newNode = mallocNode();
	
	/*Copies Keys and Pointers to the new Node*/
	for(i = (center + 1); i < last; i++) {
		newNode->key[i] = root->key[i];
		newNode->p[i] = root->p[i];
	}
	
	/*Sets end of nodes*/
	root->keysNo = center + 1;
	newNode->keysNo = last - root->keysNo;

	/*gets New Node RRN and checks leaf bit*/
	newNode->rrn = getAvail(tree);
	if( isLeaf(root) ) {
		newNode->nextLeaf = root->nextLeaf;
		root->nextLeaf = newNode->rrn;
	}
	
	/*Writes modification to file*/
	seekTree(tree, root->rrn);
	writeNode(root, tree);
	seekTree(tree, newNode->rrn);
	writeNode(newNode, tree);

	/*Returns new son*/
	return newNode;
}

treeNode * mallocNode() {
	treeNode node;

	node = malloc(sizeof(treeNode));	
	if(!node) {
		fprintf(stderr, "mallocNode: Memory Allocation Problem!\n");
		return NULL;
	}
	node->keysNo = 0;
	return node;
}

bool treeInsertKey(char * key, enum indexType keyType, int rrnsRRN, FILE * tree, FILE * rrnsFile) {
	int rootRRN;
	int lastKey;
	treeNode newSon;
	treeNode root;
	treeNode newRoot;

	
	/*Gets root RRN and loads it to memory*/
	rewind(tree);
	/*Jumps avail List*/
	fscanf("%d", &rootRRN);
	/*Reads root RRN*/
	rootRRN = getRootRRN(tree);
	seekTreeRRN(rootRRN);
	/*Loads Root to memory*/
	root = getNextNode(tree);
	
	/*Adds key, checking if root has been splitted*/
	newSon = treeInsertKey_Rec(key, keyType, rrnsRRN, root, tree, rrnsFile);
	
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
		seekTreeRRN(newRoot->rrn);
		writeNode(newRoot, tree);
		free(newRoot);
		free(newSon);
	}
	free(root);
	return true;
}
int getRootRRN(FILE * tree) {
	int rootRRN;
	
	rewind(tree);
	fscanf(tree, "%d", &rootRRN);
	return rootRRN;
}

treeNode * treeInsertKey_Rec(char * key, enum IndexType keyType, int rrnsRRN, treeNode subRoot, FILE * tree, FILE * rrnsFile) {
	treeNode * newSon = NULL;
	int nextSonRRN;
	treeNode * nextSon;
	
	/*If node is a leaf, inserts key in it*/
	if( isleaf(subRoot) ) {
		nodeInsertKey(key, rrnsRRN, keyType, subRoot, tree, rrnsFile);
		/*If leaf is full after inserting, splits it*/
		if(subRoot->keysNo == ORDER) {
			newSon = split(subRoot, tree);
		}
	}
	/*If node is not a leaf, finds the subtree where key must be inserted*/
	else {
		nextSonRRN = subRoot->p[nodeFindIndex(key, keyType, subRoot)];
		/*TODO: Check if it's NULL because allocation is done here*/
		seekTreeRRN(tree, nextSonRRN);
		nextSon = getNextNode(tree);
		newSon = treeInsertKey_Rec(key, keyType, rrnsRRN, nextSon, tree, rrnsFile);
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

void nodeInsertKey(char * key, int rrnsRRN, enum indexType keyType, treeNode * node, FILE * tree, FILE * rrnsFILE) {
	int place;
	rrnNode RRN;
	int (* cmp)();
	int i = 0, j = 0;

	/*Finds place for next key*/
	place = nodeFindIndex(key, keyType, node);

	/*Moves keys and pointers if key doesn't exist*/
	if(strcmp(node->key[place], key) != 0) {
		for(i = (place + 1); (i <= node->keysNo); i++) {
			node->key[i] = node->key[i-1];
			node->p[i] = node->p[i-1];
		}
		/*Points key to list of rrns in rrns FILE*/
		node->p[place] = rrnsRRN;
		/*writes Modification*/
		seekTreeRRN(tree, node->rrn);
		writeNode(node, tree);
	}
	/*If key already exists, add a new rrn to rrns list in file*/
	else {
		seekRrnRRR(rrnsFILE, rrnsRRN);
		rrnNode = getNextRRN(rrnsFILE);
		rrnNode->next[keyType] = node->p[place];
		node->p[place] = rrnRRN;
		/*Writes Modifications*/
		seekTreeRRN(tree, node->rrn);
		writeNode(node, tree);
		seekRrnRRN(rrnsFILE, rrnsRRN);
		writeRrn(rrnNode, rrnsFILE);
	}
}
int nodeFindIndex(char * key, enum IndexType keyType, treeNode * node) {
	int i = 0;

	switch (keyType) {
		case TITLE: case AUTHOR: case SUBJECT:
			cmp = compareWords();
			break;
		case YEAR:
			cmp = compareYear();
			break;
		case ISBN:
			cmp = compareISBN();
			break;
		default:
			fprintf(stderr, "nodeInsertKey: 
			Invalid indexType!\n -Aborting...\n");
			exit(1);
	}	
	
	/*Searches for the first key[i] >= key and stops counting*/ 
	for(i = 0; (i < node->keysNo) && (cmp(key, node->key[i]) > 0); i++);
	
	return i;
}

	
