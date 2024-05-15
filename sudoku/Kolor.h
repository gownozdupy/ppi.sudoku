#pragma once
#include "Kolor.h"
#include <windows.h>
#include <iostream>
using namespace std;

class Kolor {
public:
	Kolor(int desiredColor);

	/*Przeciazony operator <<*/
	friend ostream& operator<<(ostream& ss, Kolor obj) {
		SetConsoleTextAttribute(obj.consoleHandle, obj.kolor);
		return ss;
	}
private:
	int kolor;
	HANDLE consoleHandle;
};