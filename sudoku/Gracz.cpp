#include "Gracz.h"
#include "Sudoku.h"

Gracz::Gracz(const std::string& nick) : nick(nick) {}

const std::string& Gracz::pobierzNick() const { return nick; }
