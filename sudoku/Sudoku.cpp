#include "Kolor.h"
#include "Sudoku.h"
#include "Log.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric> // std::iota
#define NOMINMAX


using namespace std;

//Tworzy nowy obiekt sudoku o rozmiarze N x N oraz okreœlonej liczbie pustych pól K.
//Oblicza pierwiastek kwadratowy z N i przypisuje go do zmiennej SRN, który reprezentuje rozmiar kwadratowego bloku(podkwadratu) w sudoku.
//Alokuje pamiêæ dla planszy sudoku o rozmiarze N x N oraz wype³nia j¹ zerami.


Sudoku::Sudoku(int N, int K) {
    this->N = N;
    this->K = K;
    SRN = static_cast<int>(sqrt(N));
    k_indeksy = new int* [N];
    plansza = new int* [N];
    for (int i = 0; i < N; i++) {
        plansza[i] = new int[N];
        memset(plansza[i], 0, N * sizeof(int));
        k_indeksy[i] = new int[N];
        memset(k_indeksy[i], 0, N * sizeof(int));
    }
}

Sudoku::~Sudoku() {
    for (int i = 0; i < N; i++) {
        delete[] plansza[i];
    }
    delete[] plansza;
}

bool Sudoku::czyBezpieczne(int wiersz, int kolumna, int liczba) {
    for (int x = 0; x < N; x++) {
        if (plansza[wiersz][x] == liczba || plansza[x][kolumna] == liczba) {
            return false;
        }
    }

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

bool Sudoku::rozwiaz() {
    int wiersz, kolumna;
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

    if (!jestPusta) {
        return true;
    }

    for (int liczba = 1; liczba <= N; liczba++) {
        if (czyBezpieczne(wiersz, kolumna, liczba)) {
            plansza[wiersz][kolumna] = liczba;
            if (rozwiaz()) {
                return true;
            }
            plansza[wiersz][kolumna] = 0;
        }
    }

    return false;
}

void Sudoku::wypelnij() {
    wypelnijDiagonale();
    rozwiaz();
    usunKCyfr();
}

void Sudoku::wypelnijDiagonale() {
    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = 0; i < N; i += SRN) {
        vector<int> liczby(N);
        iota(liczby.begin(), liczby.end(), 1);
        std::shuffle(liczby.begin(), liczby.end(), g);
        for (int r = 0; r < SRN; r++) {
            for (int c = 0; c < SRN; c++) {
                plansza[i + r][i + c] = liczby[r * SRN + c];
            }
        }
    }
}

void Sudoku::usunKCyfr() {
    int komorkiDoUsuniecia = N * N - K;
    vector<int> pozycje(N * N);
    iota(pozycje.begin(), pozycje.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pozycje.begin(), pozycje.end(), g);

    for (int i = 0; i < komorkiDoUsuniecia; i++) {
        int wiersz = pozycje[i] / N;
        int kolumna = pozycje[i] % N;
        plansza[wiersz][kolumna] = 0;
        k_indeksy[wiersz][kolumna] = 1; //komorki do uzupelnienia przez gracza

    }
}

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

bool Sudoku::czyKomorkaWygenerowanaPrzezKomputer(int wiersz, int kolumna) {
    // SprawdŸ, czy komórka jest zerem (niezainicjowana)
    if (k_indeksy[wiersz][kolumna] == 1) {
        return false; 
    }
    else {
        return true;
    }
}


void Sudoku::graj() {
    bool grajPonownie = true;
    while (grajPonownie) {
        while (!czyUkonczone()) {
            char wiersz;
            int kolumna, liczba;
            int indeksWiersza;
            char pom = 'A' + N - 1;
            string input;
            std::cout << std::endl << "P O D A J  W S P O L R Z E D N E\n\nl i t e r a (A-" << pom << "): ";
            std::cin >> input;
            if (cin.fail() || input.length()!=1 || input[0] < 'A' || input[0] > pom) {
                system("cls");
                std::cout << "Nieprawidlowa litera. Podaj pojedyncza litere A-" << pom << ".\n" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                drukujSudoku();
                continue;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
            }
            wiersz = input[0];
            std::cout << "c y f r a (1-" << N << "): ";
            std::cin >> kolumna;

            if (cin.fail() || kolumna < 1 || kolumna > N) {
                system("cls");
                std::cout << "Nieprawidlowa cyfra. Podaj cyfre od 1 do " << N << ".\n" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                drukujSudoku();
                continue;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
            }

            std::cout << "t w o j a  o d p o w i e d z: ";
            std::cin >> liczba;

            if (cin.fail() || liczba < 1 || liczba > N) {
                system("cls");
                std::cout << "Nieprawidlowa liczba. Podaj liczbe od 1 do " << N << ".\n" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                drukujSudoku();
                continue;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
            }

            indeksWiersza = wiersz - 'A';
            kolumna--;

            if (!czyBezpieczne(indeksWiersza, kolumna, liczba)) {
                system("cls");
                std::cout << "Ta cyfra juz istnieje w rzedzie, kolumnie lub bloku. Sprobuj ponownie.\n" << std::endl;
                drukujSudoku();
            }
            if (czyKomorkaWygenerowanaPrzezKomputer(indeksWiersza, kolumna)) {
                system("cls");
                cout << "Ta cyfra zostala wygenerowana przez komputer. Nie mozna zmieniac tych wartosci. Sprobuj ponownie.\n" << endl;
                drukujSudoku();
            }
            if(czyBezpieczne(indeksWiersza, kolumna, liczba)){
                plansza[indeksWiersza][kolumna] = liczba;
                system("cls");
                drukujSudoku();
            }
        }
        system("cls");
        drukujSudoku();
        std::cout << "Gratulacje! Udalo Ci sie ukonczyc Sudoku!\n" << std::endl;
        std::cout << "\nC H C E S Z  Z A G R A C  J E S Z C Z E  R A Z?\n";
        std::cout << "\n\n1 - t a k\t 2 - n i e\n";
        std::cout << "\nT W O J  W Y B O R: ";
        int w;
        std::cin >> w;
        while (std::cin.fail() || (w != 1 && w != 2)) {
            system("cls");
            std::cout << "Nieprawidlowy wybor. Podaj 1 (tak) lub 2 (nie):\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> w;
        }

        if (w == 1) {
            system("cls");

        }
        else {
            system("cls");
            grajPonownie = false;
        }
    }
}

void Sudoku::drukujSudoku()
{
    if (floor(sqrt(N))!=sqrt(N)) {
        char lit = 'A';
        int num = 1;
        cout << "    ";
        for (num; num <= N; num++) {
            cout << setw(3) << num;
        }
        cout << endl;
        cout << setw(4) << " " << setfill('-') << setw((3 * N) + 3) << " ";
        cout << setfill(' ');
        cout << endl;

        for (int i = 0; i < N; i++) {
            cout << lit << setw(3) << "|";
            for (int j = 0; j < N; j++) {
                if (plansza[i][j] == 0) {
                    cout << setw(3) << " ";
                }
                else {
                    // SprawdŸ, czy wartoœæ jest wygenerowana przez komputer
                    if (czyKomorkaWygenerowanaPrzezKomputer(i, j)) {
                        // Ustaw kolor dla wartoœci wygenerowanych przez komputer
                        cout << setw(3) << Kolor(13) << plansza[i][j] << Kolor(7); // np. czerwony kolor
                    }
                    else {
                        // Ustaw domyœlny kolor dla pozosta³ych wartoœci
                        cout << setw(3) << Kolor(10) << plansza[i][j] << Kolor(7);
                    }
                }
                if (j + 1 == N) {
                    cout << setw(3) << "|";
                }
            }
            ++lit;
            if (i == N - 1) {
                cout << endl;
                cout << setw(4) << " " << setfill('-') << setw((3 * N) + 3) << " ";
                cout << setfill(' ');
                cout << endl;
            }
            cout << endl << endl;


        }
        cout << endl;

    }
    else {
        int dl = 3*N;//ilosc kresek -
        char lit = 'A';
        int num = 1;
        cout << "    ";
        for (num; num <= N; num++) {
            cout << setw(3) << num;
            if (num%(int)sqrt(N) == 0) {
                cout << setw(3) << " ";
                dl += 3;
            }
        }
        cout << endl;
        cout << setw(4) << " " << setfill('-') << setw(dl) << " ";
        cout << setfill(' ');
        cout << endl;

        for (int i = 0; i < N; i++) {
            cout << lit << setw(3) << "|";
            for (int j = 0; j < N; j++) {
                if (plansza[i][j] == 0) {
                    cout << setw(3) << " ";
                }
                else {
                    // SprawdŸ, czy wartoœæ jest wygenerowana przez komputer
                    if (czyKomorkaWygenerowanaPrzezKomputer(i, j)) {
                        // Ustaw kolor dla wartoœci wygenerowanych przez komputer
                        cout << setw(3) << Kolor(13) << plansza[i][j] << Kolor(7); // np. czerwony kolor
                    }
                    else {
                        // Ustaw domyœlny kolor dla pozosta³ych wartoœci
                        cout << setw(3) << Kolor(10) << plansza[i][j] << Kolor(7);
                    }
                }
                if ((j+1)%(int)sqrt(N)==0||j==N+1) {
                    cout << setw(3) << "|";
                }
            }
            ++lit;
            if ((i + 1) % (int)sqrt(N) == 0||i == N - 1) {
                cout << endl;
                cout << setw(4) << " " << setfill('-') << setw(dl) << " ";
                cout << setfill(' ');
               cout << endl;
            }
            else
            cout << endl << endl;
    
        
        }
        cout << endl;

    }
}
