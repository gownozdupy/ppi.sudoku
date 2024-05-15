// Poprawki do Menu.cpp

#include "Menu.h"
#include "Sudoku.h"
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

    cout << "Sudoku sklada sie z siatki NxN pol (do wyboru przez gracza).\nUzyj tylko cyfr od 1 do N.\nKazdy blok NxN moze zawierac tylko cyfry od 1 do N.\n";
    cout << "Kazda pionowa kolumna moze zawierac tylko cyfry od 1 do N.\nKazdy poziomy wiersz moze zawierac tylko cyfry od 1 do N.\n";
    cout << "Kazda cyfra w bloku sqrt(N)xsqrt(N), pionowej kolumnie lub poziomym wierszu moze byc uzyta tylko raz.\n";
    cout << "Gra konczy sie, gdy caly diagram Sudoku jest poprawnie wypelniony cyframi.\n";
    cout << "Sa trzy poziomy trudnosci - latwy (47% planszy odkryte), sredni (44% planszy odkryte) i trudny (37% planszy odkryte).\nMozesz rowniez wybrac opcje automatycznego uzupelnienia planszy.\n";
    cout << "Milej gry!\n\n\n";
    //cout << "Kliknij dowolny przycisk aby wrocic do menu.\n";
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
            cin >> wyborMenu;
            //wprowadz kontrole poprawnosci danych
        switch (wyborMenu) {
        case 1://wyswietl zasady
            printinfo();
            break;
        case 2://gra
        {
            system("cls");
            cout << "=================================================================\n";
            cout << "W Y B I  E R Z  P O Z I O M  T R U D N O S C I:\n";
            cout << "1. l a t w y\n";
            cout << "2. s r e d n i\n";
            cout << "3. t r u d n y\n";
            cout << "=================================================================\n";
            cout << "\n\nT W O J  W Y B O R : ";
            int poziomTrudnosci;
            cin >> poziomTrudnosci;
            cout << "Podaj nick: ";
            string nick;
            cin >> nick;

            Gracz gracz(nick);
            system("CLS");
            cout << "=================================================================\n";
            cout << nick << ", W Y B I E R Z  W Y M I A R  P L A N S Z Y (np. 9x9): \n";
            cout << "=================================================================\n";
            cout << "\n\nT W O J  W Y B O R : ";
            int N;
            cin >> N;
            system("CLS");
            cout << "=================================================================\n";
            cout << nick << ", W Y B I E R Z  T R Y B  G R Y:\n1. G R A  S A M O D Z I E L N A\n2. K O M P U T E R\n";
            cout << "=================================================================\n";
            cout << "\n\nT W O J  W Y B O R : ";
            int wybor;
            cin >> wybor;
            system("CLS");

            switch (wybor) {//gracz
            case 1: {
                switch (poziomTrudnosci) {
                case 1://latwy
                {
                    int K = 0.37 * N * N;
                    Sudoku sudoku1(N, K);
                    sudoku1.wypelnij();
                    sudoku1.drukujSudoku();
                    sudoku1.graj();
                }
                case 2://sredni
                {
                    int K = 0.44 * N * N;
                    Sudoku sudoku2(N, K);
                    sudoku2.wypelnij();
                    sudoku2.drukujSudoku();
                    sudoku2.graj();
                }
                case 3://trudny
                {
                    int K = 0.47 * N * N;
                    Sudoku sudoku3(N, K);
                    sudoku3.wypelnij();
                    sudoku3.drukujSudoku();
                    sudoku3.graj();
                }
                }
            }
            case 2: {//komputer
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
            }
            }
        }
        case 3:
            i = false;
            break;
        }
    }
}