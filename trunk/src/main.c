#include <stdlib.h>
#include <getopt.h>
#include "catalog.h"
#include "interfaces.h"
#include "tools.h"

const char * program_name;

int main(int argc, char* argv[]) {
	int opt_index, opt;
	int exit_code = 0;
	char * catalogName;
	const char* short_opt = "a:c:hiq:l:o:r:";
	const struct option long_opt[] = {
		{"create", 1, 0, 'c'},
		{"help", 0, 0, 'h'},
		{"interactive", 0, 0, 'i'},
		{"query", 1, 0, 'q'},
		{"list", 1, 0, 'l'},
		{"add", 1, 0, 'a'},
		{"out", 1, 0, 'o'},
		{"remove", 1, 0, 'r'},
		{0,0,0,0}
		};

		program_name = argv[0];

		switch (NEXT_OPT) {
			case 'a':
				nonInteractiveInterface(argc, argv);
				break;
			case 'c':
				catalogName = optarg;
				if(createFile(catalogName, "a+"))
					printf("'%s' Catalog created\n", catalogName);
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
				exit_code = ! nonInteractiveQuery(argc, argv);
				break;
			case 'l':
				catalogName = optarg;
				if(NEXT_OPT == 'o') {
					generateList(catalogName, optarg);
				}
				else {
					fprintf(stderr, "Missing output file name\n");
					exit_code = 1;
				}
				break;
			case 'r':
				/*Implementation Process*/
				nonInteractiveRemoval(optarg, argc, argv);
				break;
			default:
				fprintf(stderr, "-h for help. Parameters required\n");
				break;
		}

		exit (exit_code);
		}
