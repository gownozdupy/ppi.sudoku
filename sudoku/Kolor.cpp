#include "Kolor.h"

Kolor::Kolor(int desiredKolor) {
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	kolor = desiredKolor;
}