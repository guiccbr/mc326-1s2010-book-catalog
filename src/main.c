#include "catalog.h"
#include "tools.h"

int main(int argc, char* argv[]) {
	int opt_index; int opt;
	const char* short_opt = "a:c:hiq:l:";
	const struct option long_opt[] = {
		{"create", 1, 0, 'c'},
		{"help", 0, 0, 'h'},
		{"interactive", 0, 0, 'i'},
		{"query", 0, 0, 'q'},
		{"list", 0, 0, 'l'},
		{"add", 1, 0, 'a'},
		{0,0,0,0}
    };
	    
	switch (NEXT_OPT) {
		case 'a':
			nonInteractiveInterface(argc, argv);
			break;
		case 'c':
		/*Funcao eh capaz de sobrescrever qualquer arquivo - o que eh extremamente inseguro
		* TODO: Consertar isso*/
			if(createFile(optarg, "a+"))
				printf("'%s' Catalog created\n", optarg);
			break;
		case 'h':
			printFile(HELP_FILE);
			break;
		case 'i':
			interactiveInterface();
			break;
		case 'q':
			query();
			break;
		case 'l':
			printList(optarg);
			break;
		default:
			fprintf(stderr, "-h for help. Parameters required\n");
			break;	
	}
	exit (0);
}
