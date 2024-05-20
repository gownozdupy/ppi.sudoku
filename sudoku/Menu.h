#pragma once
#ifndef MENU_H
#define MENU_H
#define NOMINMAX

#include <string>

class Menu
{
private:
    int wyborMenu; // Dodajemy pole do przechowywania wyboru u¿ytkownika
public:
    int getWyborMenu() const;
    void printinfo();
    void printmenu();
    void interface();
};

#endif
