#include <ctime>
#include <fstream>
#include "Log.h"


void Log::zapiszLog(std::string nick, int poziomTrudnosci, int liczba, double procentRozwiazania) {
    time_t now;
    struct tm timeinfo;
    char buffer[80];

    time(&now);
    localtime_s(&timeinfo, &now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string str(buffer);

    // Zapisz log do pliku
    std::ofstream file(nick + "_log.txt", std::ios::app);    if (file.is_open()) {
        file << "Data: " << str << ", Nick: " << nick << ", Poziom trudnosci: " << poziomTrudnosci << ", Podana liczba: " << liczba << ", Procent rozwiazania: " << procentRozwiazania << " % " << std::endl;
        file.close();
    }
}

void Log::zapiszLog(std::string nick,int N) {
    time_t now;
    struct tm timeinfo;
    char buffer[80];

    time(&now);
    localtime_s(&timeinfo, &now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string str(buffer);

    // Zapisz log do pliku
    std::ofstream file(nick + "_log.txt", std::ios::app);
    if (file.is_open()) {
        file << "Data: " << str << ", Nick: " << nick << "Wybrano wymiar planszy: " << N;
        file.close();
    }
}

void Log::zapiszLog(std::string nick, char a) {
    time_t now;
    struct tm timeinfo;
    char buffer[80];

    time(&now);
    localtime_s(&timeinfo, &now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string str(buffer);

    // Zapisz log do pliku
    std::ofstream file(nick + "_log.txt", std::ios::app);
    if (file.is_open()) {
        file << "Data: " << str << ", Nick: " << nick << "Podano bledne wspolrzedna: " << a;
        file.close();
    }
}