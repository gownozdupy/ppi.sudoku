#pragma once
#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>

class Log {
public:
    void zapiszLog(std::string nick, int poziomTrudnosci, int liczba, double procentRozwiazania); 
    void zapiszLog(std::string nick, int N);
    void zapiszLog(std::string nick, char a);
};
#endif