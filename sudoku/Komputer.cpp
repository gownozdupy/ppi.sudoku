#include "Komputer.h"
#include <iostream>
 bool Komputer::rozwiazSudoku(Sudoku* sudoku) {
    std::cout << "\n\nKomputer rozwiazuje Sudoku...\n\n";
    sudoku->rozwiaz();
    sudoku->drukujSudoku();
    return true;
}
