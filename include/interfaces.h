#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "books.h"
#include "catalog.h"
#include "menus.h"
#include "tools.h"

/*Shows interactiveInterface, for runtime user's input*/
void interactiveInterface();

/*Reads arguments from shell's program call*/
void nonInteractiveInterface(int argc, char* argv[]);
