#include "Sudoku.h"
#include "Log.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <assert.h>
#include <cstdlib> 
#include <random>
#include <algorithm>
//#include "Kolor.h"


using namespace std;

//Tworzy nowy obiekt sudoku o rozmiarze N x N oraz okreœlonej liczbie pustych pól K.
//Oblicza pierwiastek kwadratowy z N i przypisuje go do zmiennej SRN, który reprezentuje rozmiar kwadratowego bloku(podkwadratu) w sudoku.
//Alokuje pamiêæ dla planszy sudoku o rozmiarze N x N oraz wype³nia j¹ zerami.

Sudoku::Sudoku(int N, int K) {
    this->N = N;
    this->K = K;
    double SRNd = sqrt(N);
    SRN = static_cast<int>(SRNd);
    plansza = new int* [N];
    for (int i = 0; i < N; i++) {
        plansza[i] = new int[N];
        memset(plansza[i], 0, N * sizeof(int));
    }
}

//DESTRUKTOR

Sudoku::~Sudoku() {
    for (int i = 0; i < N; i++) {
        delete[] plansza[i];
    }
    delete[] plansza;
}

//Sprawdza, czy dana liczba mo¿e byæ bezpiecznie umieszczona w okreœlonej komórce, uwzglêdniaj¹c warunki sudoku(brak tej samej liczby w tym samym wierszu, kolumnie i kwadracie o rozmiarze SRN x SRN).

bool Sudoku::czyBezpieczne(int wiersz, int kolumna, int liczba) {
    // SprawdŸ, czy liczba nie wystêpuje w tym samym wierszu lub kolumnie
    for (int x = 0; x < N; x++) {
        if (plansza[wiersz][x] == liczba || plansza[x][kolumna] == liczba) {
            return false;
        }
    }

    // SprawdŸ, czy liczba nie wystêpuje w podkwadracie SRN x SRN
    int poczatekWiersza = wiersz - wiersz % SRN;
    int poczatekKolumny = kolumna - kolumna % SRN;
    for (int i = 0; i < SRN; i++) {
        for (int j = 0; j < SRN; j++) {
            if (plansza[i + poczatekWiersza][j + poczatekKolumny] == liczba) {
                return false;
            }
        }
    }

    return true;
}
//Rozwi¹zuje sudoku przy u¿yciu algorytmu backtrackingu.
//Szuka pustej komórki i próbuje wype³niæ j¹ liczbami od 1 do N.Jeœli znajdzie bezpieczn¹ liczbê, to kontynuuje rekurencyjnie rozwiazywanie reszty sudoku.Jeœli nie, cofa siê i próbuje kolejn¹ liczbê.

bool Sudoku::rozwiaz() {
    int wiersz, kolumna;

    // ZnajdŸ pust¹ komórkê
    bool jestPusta = false;
    for (wiersz = 0; wiersz < N; wiersz++) {
        for (kolumna = 0; kolumna < N; kolumna++) {
            if (plansza[wiersz][kolumna] == 0) {
                jestPusta = true;
                break;
            }
        }
        if (jestPusta) {
            break;
        }
    }

    // Jeœli nie ma wiêcej pustych komórek, zakoñcz
    if (!jestPusta) {
        return true;
    }

    // Wypróbuj wype³nienie komórki
    for (int liczba = 1; liczba <= N; liczba++) {
        if (czyBezpieczne(wiersz, kolumna, liczba)) {
            plansza[wiersz][kolumna] = liczba;
            if (rozwiaz()) {
                return true;
            }
            plansza[wiersz][kolumna] = 0; // Wycofaj zmiany i spróbuj inn¹ liczbê
        }
    }

    // Brak mo¿liwych rozwi¹zañ
    return false;
}


//Wywo³uje wypelnijDiagonale() by umieœciæ losowo liczby na diagonalach.
//Nastêpnie próbuje rozwi¹zaæ sudoku u¿ywaj¹c metody rozwiaz().
//Usuwa pewn¹ liczbê cyfr(okreœlon¹ przez K) by utworzyæ planszê sudoku do rozwi¹zania.

void Sudoku::wypelnij() {
    wypelnijDiagonale();
    rozwiaz();
    usunKCyfr();
}

//Wype³nia diagonalne bloki sudoku liczbami z przedzia³u od 1 do N, sprawdzaj¹c, czy s¹ one bezpieczne.

void Sudoku::wypelnijDiagonale() {
    for (int i = 0; i < N; i += SRN) {
        for (int liczba = 1; liczba <= N; liczba++) {
            int wiersz = i + rand() % SRN;
            int kolumna = i + rand() % SRN;
            while (!czyBezpieczne(wiersz, kolumna, liczba)) {
                wiersz = i + rand() % SRN;
                kolumna = i + rand() % SRN;
            }
            plansza[wiersz][kolumna] = liczba;
        }
    }
}


//Usuwa losowo wybrane cyfry z uzupe³nionej planszy, tworz¹c planszê do rozwi¹zania.

void Sudoku::usunKCyfr() {
    int komorkiDoUsuniecia = N * N - K;
    vector<int> pozycje;
    for (int i = 0; i < N * N; i++) {
        pozycje.push_back(i);
    }
    
    // U¿ywamy generatora liczb pseudolosowych z biblioteki random
    std::random_device rd;
    std::mt19937 g(rd());

    // U¿ywamy funkcji shuffle zamiast random_shuffle
    std::shuffle(pozycje.begin(), pozycje.end(), g);
    
    for (int i = 0; i < komorkiDoUsuniecia; i++) {
        int wiersz = pozycje[i] / N;
        int kolumna = pozycje[i] % N;
        plansza[wiersz][kolumna] = 0;
    }
}


//Sprawdza, czy plansza sudoku zosta³a wype³niona w ca³oœci.

bool Sudoku::czyUkonczone() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (plansza[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

//Oblicza procent rozwi¹zania planszy sudoku na podstawie liczby uzupe³nionych komórek i ca³kowitego rozmiaru planszy.

double obliczProcentRozwiazania(int i, int N) {
    return (i / N)*100;
}

void Sudoku::graj() {

    while (!czyUkonczone()) {
        char wiersz;
        int kolumna, liczba;
        int indeksWiersza;

        std::cout << std::endl << "P O D A J  W S P O L R Z E D N E\n\nl i t e r a (A-I): ";
        std::cin >> wiersz;

        // SprawdŸ, czy wiersz jest liter¹ A-I
        if (wiersz < 'A' || wiersz > 'I') {
            system("CLS");
            std::cout << "\nNieprawidlowa litera. Podaj litere A-I.\n" << std::endl;
            std::cin.clear(); // Wyczyœæ b³¹d strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczyœæ b³êdne dane z bufora wejœcia
            drukujSudoku();
            continue; // Powrót do pocz¹tku pêtli
        }

        std::cout << "c y f r a (1-9): ";
        std::cin >> kolumna;

        //// SprawdŸ, czy kolumna mieœci siê w zakresie 1-9
        if (kolumna < 1 || kolumna > 9) {
            system("CLS");

            std::cout << "\nNieprawidlowa cyfra. Podaj cyfre od 1 do 9.\n" << std::endl;
            std::cin.clear(); // Wyczyœæ b³¹d strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczyœæ b³êdne dane z bufora wejœcia
            drukujSudoku();

            continue; // Powrót do pocz¹tku pêtli
        }

        std::cout << "t w o j a  o d p o w i e d z: ";
        std::cin >> liczba;

        // SprawdŸ, czy liczba mieœci siê w zakresie 1-9
        if (liczba < 1 || liczba > 9) {
            system("CLS");

            std::cout << "\nNieprawidlowa liczba. Podaj liczbe od 1 do 9.\n" << std::endl;
            std::cin.clear(); // Wyczyœæ b³¹d strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczyœæ b³êdne dane z bufora wejœcia
            drukujSudoku();

            continue; // Powrót do pocz¹tku pêtli
        }

        indeksWiersza = wiersz - 'A';
        kolumna--;

        if (!czyBezpieczne(indeksWiersza, kolumna, liczba)) {
            system("CLS");
            std::cout << "\n\nTa cyfra juz istnieje w rzedzie, kolumnie lub bloku. Sprobuj ponownie.\n" << std::endl;
            drukujSudoku();
        }
        else {
            plansza[indeksWiersza][kolumna] = liczba;
            int i = N - K + 1;
            system("cls");
            drukujSudoku();
        }
    }

    system("cls");
    std::cout << "Gratulacje! Udalo Ci sie ukonczyc Sudoku!\n" << std::endl;
    std::cout << "\nC H C E S Z  Z A G R A C  J E S Z C Z E  R A Z?\n";
    std::cout << "\n\n1 - t a k\t 2 - n i e\n";
    std::cout << "\nT W O J  W Y B O R: ";
    int w;
    std::cin >> w;
    while ((w != 1) || (w != 2)) {
        system("CLS");
        std::cout << "\nNieprawidlowy wybor. Podaj cyfre 1 lub 2.\n" << std::endl;
        std::cin.clear(); // Wyczyœæ b³¹d strumienia
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczyœæ b³êdne dane z bufora wejœcia
        std::cout << "\nC H C E S Z  Z A G R A C  J E S Z C Z E  R A Z?\n";
        std::cout << "\n\n1 - t a k\t 2 - n i e\n";
        std::cout << "\n\nT W O J  W Y B O R: ";
        cin >> w;
    }

    system("CLS");
    if (w == 2) {
        return;
    }
    else {
        !czyUkonczone();
    }
}



void Sudoku::drukujSudoku()
{    
    char lit = 'A';
   double num = 1;
    cout << "    ";
    for (num; num <= N; num++) {
        cout << setw(3) << num;
        if (num == N/sqrt(N) || num == (N/sqrt(N))*2 || num == (N/sqrt(N))*3) {
            cout << "   ";
        }
    }
    cout << endl;
    cout << setw(41) << "-------------------------------------\n";
    for (int i = 0; i < N; i++) {
        cout << lit << setw(3) << "|";
        for (int j = 0; j < N; j++) {
            if (plansza[i][j] == 0) {
                cout << setw(3) << " ";
            }
            else {
                cout << setw(3) << /*Kolor(16) << */ plansza[i][j]; //<< Kolor(7);

            }
            if (j == 2 || j == 5 || j == 8)
                cout << setw(3) << "|";
        }
        ++lit;
        cout << endl << endl;
        if (i == (N / sqrt(N))-1 || i == ((N / sqrt(N)) * 2)-1 || i == ((N / sqrt(N)) * 3)-1)
            cout << setw(41) << "-------------------------------------\n";

    }
    cout << endl;
}
