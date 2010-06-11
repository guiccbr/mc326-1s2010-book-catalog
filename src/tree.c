#include <tools.h>

typedef struct node {
	char * key[ORDER];
	int pointer[ORDER];
	int nextLeaf;
	int keysNo;
	int rrn;
}treeNode;

#define isLeaf((node)) ((node)->nextLeaf != -2)

typedef struct rrn {
	int rrn;
	int yearList;
	int authorList;
	int subjectList;
	int titleList;
	int isbn;
}

treeNode split(treeNode * root, FILE * tree) {
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
	newNode->rrn = getAvail();
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

