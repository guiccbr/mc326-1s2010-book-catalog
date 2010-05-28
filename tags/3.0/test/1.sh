#!/bin/bash
# Simple test: add a book with valid input

./catalog -a catalogo00-wh.dat \
	-A "George Orwell" \
	-t 1984 \
	-I 1234567890123 \
	-s "A dystopian novel about the totalitarian regime of the Party, an oligarchical collectivist society where life in the Oceanian province of Airstrip One is a world of perpetual war, pervasive government surveillance, public mind control, and the voiding of citizens' rights." \
	-c "Winston Smith,Julia" \
	-y 1949 \
	-S "Fiction" \
	-i 011200.png
