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

//Tworzy nowy obiekt sudoku o rozmiarze N x N oraz okre�lonej liczbie pustych p�l K.
//Oblicza pierwiastek kwadratowy z N i przypisuje go do zmiennej SRN, kt�ry reprezentuje rozmiar kwadratowego bloku(podkwadratu) w sudoku.
//Alokuje pami�� dla planszy sudoku o rozmiarze N x N oraz wype�nia j� zerami.

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

//Sprawdza, czy dana liczba mo�e by� bezpiecznie umieszczona w okre�lonej kom�rce, uwzgl�dniaj�c warunki sudoku(brak tej samej liczby w tym samym wierszu, kolumnie i kwadracie o rozmiarze SRN x SRN).

bool Sudoku::czyBezpieczne(int wiersz, int kolumna, int liczba) {
    // Sprawd�, czy liczba nie wyst�puje w tym samym wierszu lub kolumnie
    for (int x = 0; x < N; x++) {
        if (plansza[wiersz][x] == liczba || plansza[x][kolumna] == liczba) {
            return false;
        }
    }

    // Sprawd�, czy liczba nie wyst�puje w podkwadracie SRN x SRN
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
//Rozwi�zuje sudoku przy u�yciu algorytmu backtrackingu.
//Szuka pustej kom�rki i pr�buje wype�ni� j� liczbami od 1 do N.Je�li znajdzie bezpieczn� liczb�, to kontynuuje rekurencyjnie rozwiazywanie reszty sudoku.Je�li nie, cofa si� i pr�buje kolejn� liczb�.

bool Sudoku::rozwiaz() {
    int wiersz, kolumna;

    // Znajd� pust� kom�rk�
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

    // Je�li nie ma wi�cej pustych kom�rek, zako�cz
    if (!jestPusta) {
        return true;
    }

    // Wypr�buj wype�nienie kom�rki
    for (int liczba = 1; liczba <= N; liczba++) {
        if (czyBezpieczne(wiersz, kolumna, liczba)) {
            plansza[wiersz][kolumna] = liczba;
            if (rozwiaz()) {
                return true;
            }
            plansza[wiersz][kolumna] = 0; // Wycofaj zmiany i spr�buj inn� liczb�
        }
    }

    // Brak mo�liwych rozwi�za�
    return false;
}


//Wywo�uje wypelnijDiagonale() by umie�ci� losowo liczby na diagonalach.
//Nast�pnie pr�buje rozwi�za� sudoku u�ywaj�c metody rozwiaz().
//Usuwa pewn� liczb� cyfr(okre�lon� przez K) by utworzy� plansz� sudoku do rozwi�zania.

void Sudoku::wypelnij() {
    wypelnijDiagonale();
    rozwiaz();
    usunKCyfr();
}

//Wype�nia diagonalne bloki sudoku liczbami z przedzia�u od 1 do N, sprawdzaj�c, czy s� one bezpieczne.

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


//Usuwa losowo wybrane cyfry z uzupe�nionej planszy, tworz�c plansz� do rozwi�zania.

void Sudoku::usunKCyfr() {
    int komorkiDoUsuniecia = N * N - K;
    vector<int> pozycje;
    for (int i = 0; i < N * N; i++) {
        pozycje.push_back(i);
    }
    
    // U�ywamy generatora liczb pseudolosowych z biblioteki random
    std::random_device rd;
    std::mt19937 g(rd());

    // U�ywamy funkcji shuffle zamiast random_shuffle
    std::shuffle(pozycje.begin(), pozycje.end(), g);
    
    for (int i = 0; i < komorkiDoUsuniecia; i++) {
        int wiersz = pozycje[i] / N;
        int kolumna = pozycje[i] % N;
        plansza[wiersz][kolumna] = 0;
    }
}


//Sprawdza, czy plansza sudoku zosta�a wype�niona w ca�o�ci.

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

//Oblicza procent rozwi�zania planszy sudoku na podstawie liczby uzupe�nionych kom�rek i ca�kowitego rozmiaru planszy.

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

        // Sprawd�, czy wiersz jest liter� A-I
        if (wiersz < 'A' || wiersz > 'I') {
            system("CLS");
            std::cout << "\nNieprawidlowa litera. Podaj litere A-I.\n" << std::endl;
            std::cin.clear(); // Wyczy�� b��d strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczy�� b��dne dane z bufora wej�cia
            drukujSudoku();
            continue; // Powr�t do pocz�tku p�tli
        }

        std::cout << "c y f r a (1-9): ";
        std::cin >> kolumna;

        //// Sprawd�, czy kolumna mie�ci si� w zakresie 1-9
        if (kolumna < 1 || kolumna > 9) {
            system("CLS");

            std::cout << "\nNieprawidlowa cyfra. Podaj cyfre od 1 do 9.\n" << std::endl;
            std::cin.clear(); // Wyczy�� b��d strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczy�� b��dne dane z bufora wej�cia
            drukujSudoku();

            continue; // Powr�t do pocz�tku p�tli
        }

        std::cout << "t w o j a  o d p o w i e d z: ";
        std::cin >> liczba;

        // Sprawd�, czy liczba mie�ci si� w zakresie 1-9
        if (liczba < 1 || liczba > 9) {
            system("CLS");

            std::cout << "\nNieprawidlowa liczba. Podaj liczbe od 1 do 9.\n" << std::endl;
            std::cin.clear(); // Wyczy�� b��d strumienia
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczy�� b��dne dane z bufora wej�cia
            drukujSudoku();

            continue; // Powr�t do pocz�tku p�tli
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
        std::cin.clear(); // Wyczy�� b��d strumienia
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyczy�� b��dne dane z bufora wej�cia
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
