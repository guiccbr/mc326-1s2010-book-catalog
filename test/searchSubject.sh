#!/bin/bash
# Subject Search test.

# Expected result: file test/TestOut.html will be created according to model
# default.html with every book that contains "Quadrinhos" in its subject.
./catalog -q catalogo00.dat -S Quadrinhos -m default.html -o TestOut.html
