options = -g -O2 -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodeminimodel -lgecodegist -lgecodedriver -lQtCore -lQtGui  -I/usr/include/qt4 

sudoku: sudoku.cpp
	g++ $(options) sudoku.cpp -o sudoku

queens: queens.cpp
	g++ $(options) queens.cpp -o queens
