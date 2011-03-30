options = -g -O2 -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodeminimodel

sudoku: sudoku.cpp
	g++ $(options) sudoku.cpp -o sudoku
