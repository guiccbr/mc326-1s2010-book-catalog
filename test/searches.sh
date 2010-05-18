#!/bin/bash
# Search tests.

# ISBN search.
# Expected result: file test/TestOut.html will be created according to model default.html
# with information from book "Jimmy Corrigan - O Menino Mais Esperto do Mundo".
./catalog -q catalogo00.dat -i 9788535915488 -m default.html -o test/TestOut.html
