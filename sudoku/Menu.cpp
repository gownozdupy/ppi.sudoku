#include "Sudoku.h"
#include "Menu.h"
#include "Log.h" // Dodajemy nag³ówek Log.h
#include "Komputer.h"
#include "Gracz.h"
#include <iostream>
#include <iomanip>
#include <cstdlib> // Dodajemy bibliotekê, aby móc korzystaæ z funkcji system()
#include <cmath>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>
#include <conio.h>

using namespace std;

// Metoda do pobierania wyboru u¿ytkownika
int Menu::getWyborMenu() const {
    return wyborMenu;
}

void Menu::printinfo() {
    system("cls");
    cout << "========================================\n";
    cout << setw(30) << "Z A S A D Y  G R Y: \n";
    cout << "========================================\n";

    cout << "Sudoku sklada sie z siatki NxN pol (do wyboru przez gracza - 3x3, 4x4, 9x9, 16x16).\n";
    cout << "Uzyj tylko cyfr od 1 do N.\n";
    cout << "Kazdy blok NxN moze zawierac tylko cyfry od 1 do N.\n";
    cout << "Kazda pionowa kolumna moze zawierac tylko cyfry od 1 do N.\n";
    cout << "Kazdy poziomy wiersz moze zawierac tylko cyfry od 1 do N.\n";
    cout << "Kazda cyfra w bloku sqrt(N)xsqrt(N), pionowej kolumnie lub poziomym wierszu moze byc uzyta tylko raz.\n";
    cout << "Gra konczy sie, gdy caly diagram Sudoku jest poprawnie wypelniony cyframi.\n";
    cout << "Sa trzy poziomy trudnosci:\n";
    cout << "1. latwy(47 % planszy odkryte),\n2. sredni(44 % planszy odkryte),\n3.trudny(37 % planszy odkryte).\nMozesz rowniez wybrac opcje automatycznego uzupelnienia planszy.\n";
    cout << "========================================\n";
    cout << setw(30) << "M I L E J  G R Y! \n";
    cout << "========================================\n";
    cout << "\nKliknij dowolny przycisk aby wrocic do menu.\n";
    char znak = _getch();
    if (znak) {
        system("CLS");
        return;
    }
}

void Menu::printmenu() {
    cout << "========================================\n";
    cout << setw(24) << "Witaj!";
    cout << "\n========================================\n\n";
    cout << setw(27) << "M  E  N  U\n";
    cout << "\n========================================\n\n";
    cout << "1. i n s t r u k c j a\n";
    cout << "2. z a g r a j\n";
    cout << "3. w y j d z\n";
    cout << "\n\nT W O J  W Y B O R : ";
}


void Menu::interface() {
    bool i = true;
    while (i) {
        printmenu();
        int wyborMenu;
        while (true) {
            cin >> wyborMenu;
            if (cin.fail() || (wyborMenu < 1 || wyborMenu > 3)) {
                cout << "Niepoprawny wybor. Wprowadz liczbe od 1 do 3: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
                break;
            }
        }

        switch (wyborMenu) {
        case 1: // wyswietl zasady
            printinfo();
            break;
        case 2: // gra
        {
            system("cls");
            cout << "=================================================================\n";
            cout << "W Y B I E R Z  P O Z I O M  T R U D N O S C I:\n";
            cout << "1. l a t w y\n";
            cout << "2. s r e d n i\n";
            cout << "3. t r u d n y\n";
            cout << "=================================================================\n";
            cout << "\n\nT W O J  W Y B O R : ";
            int poziomTrudnosci;
            while (true) {
                cin >> poziomTrudnosci;
                if (cin.fail() || (poziomTrudnosci < 1 || poziomTrudnosci > 3)) {
                    cout << "Niepoprawny wybor. Wprowadz liczbe od 1 do 3: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
                    break;
                }
            }

            cout << "Podaj nick: ";
            string nick;
            cin >> nick;

            Gracz gracz(nick);
            system("CLS");
            cout << "=================================================================\n";
            cout << nick << ", W Y B I E R Z  W Y M I A R  P L A N S Z Y: \n";
            cout << "=================================================================\n";
            cout << "\n\n1. 3 x 3\n";
            cout << "\n2. 4 x 4\n";
            cout << "\n3. 9 x 9\n";
            cout << "\n4. 16 x 16\n";
            cout << "\n\nT W O J  W Y B O R : ";
            int wyb;
            int N;
            while (true) {
                cin >> wyb;
                if (cin.fail() || (wyb < 1 || wyb > 4)) {
                    cout << "Niepoprawny wybor. Wprowadz liczbe od 1 do 4: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
                    break;
                }
            }

            switch (wyb) {
            case 1: N = 3; break;
            case 2: N = 4; break;
            case 3: N = 9; break;
            case 4: N = 16; break;
            }

            system("CLS");
            cout << "=================================================================\n";
            cout << nick << ", W Y B I E R Z  T R Y B  G R Y:\n1. G R A  S A M O D Z I E L N A\n2. K O M P U T E R\n";
            cout << "=================================================================\n";
            cout << "\n\nT W O J  W Y B O R : ";
            int wybor;
            while (true) {
                cin >> wybor;
                if (cin.fail() || (wybor < 1 || wybor > 2)) {
                    cout << "Niepoprawny wybor. Wprowadz liczbe od 1 do 2: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usuniêcie ewentualnych pozosta³oœci z bufora
                    break;
                }
            }
            system("CLS");

            switch (wybor) { // gracz
            case 1: {
                switch (poziomTrudnosci) {
                case 1: { // latwy
                    int K = 0.37 * N * N;
                    Sudoku sudoku1(N, K);
                    sudoku1.wypelnij();
                    sudoku1.drukujSudoku();
                    sudoku1.graj();
                    break;
                }
                case 2: { // sredni
                    int K = 0.44 * N * N;
                    Sudoku sudoku2(N, K);
                    sudoku2.wypelnij();
                    sudoku2.drukujSudoku();
                    sudoku2.graj();
                    break;
                }
                case 3: { // trudny
                    int K = 0.47 * N * N;
                    Sudoku sudoku3(N, K);
                    sudoku3.wypelnij();
                    sudoku3.drukujSudoku();
                    sudoku3.graj();
                    break;
                }
                }
                break;
            }
            case 2: { // komputer
                switch (poziomTrudnosci) {
                case 1: {
                    int K = 0.37 * N * N;
                    Sudoku sudoku1(N, K);
                    sudoku1.wypelnij();
                    sudoku1.drukujSudoku();
                    Komputer komputer;
                    auto start = chrono::steady_clock::now();
                    if (!komputer.rozwiazSudoku(&sudoku1)) {
                        cout << "Nie udalo sie rozwiazac Sudoku.";
                        return;
                    }
                    auto end = chrono::steady_clock::now();
                    chrono::duration<double> elapsed_seconds = end - start;

                    sudoku1.drukujSudoku();
                    break;
                }
                case 2: {
                    int K = 0.44 * N * N;
                    Sudoku sudoku2(N, K);
                    sudoku2.wypelnij();
                    sudoku2.drukujSudoku();

                    Komputer komputer;
                    auto start = chrono::steady_clock::now();
                    if (!komputer.rozwiazSudoku(&sudoku2)) {
                        cout << "Nie udalo sie rozwiazac Sudoku.";
                        return;
                    }
                    auto end = chrono::steady_clock::now();
                    chrono::duration<double> elapsed_seconds = end - start;

                    sudoku2.drukujSudoku();
                    break;
                }
                case 3: {
                    int K = 0.47 * N * N;
                    Sudoku sudoku3(N, K);
                    sudoku3.wypelnij();
                    sudoku3.drukujSudoku();

                    Komputer komputer;
                    auto start = chrono::steady_clock::now();
                    if (!komputer.rozwiazSudoku(&sudoku3)) {
                        cout << "Nie udalo sie rozwiazac Sudoku.";
                        return;
                    }
                    auto end = chrono::steady_clock::now();
                    chrono::duration<double> elapsed_seconds = end - start;

                    sudoku3.drukujSudoku();
                    break;
                }
                }
                break;
            }
            }
            break;
        }
        case 3:
            i = false;
            break;
        }
    }
}
