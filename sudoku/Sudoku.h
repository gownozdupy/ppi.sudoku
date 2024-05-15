#ifndef SUDOKU_H
#define SUDOKU_H

class Sudoku {
public:
    int** plansza;
    int N;
    int SRN;
    int K;

    Sudoku(int N, int K);
    ~Sudoku();
    bool czyUkonczone();
    bool rozwiaz();
    void graj();
    void wypelnij();
    double obliczProcentRozwiazania(int i, int N);
    void wypelnijDiagonale();
    bool czyBezpieczne(int i, int j, int liczba);
    void usunKCyfr();
    void drukujSudoku();
};

#endif
