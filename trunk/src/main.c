#include "libs.h"

int main(int argc, char* argv[]) {
	int opt_index, opt;
	int exit_code = 0;
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
			if(createFile(optarg, "a+"))
				printf("'%s' Catalog created\n", optarg);
			else {
				fprintf(stderr, "Unable to create catalog\n");
				exit_code = 1;
			}
			break;
		case 'h':
			printHelp();
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
	exit (exit_code);
}
