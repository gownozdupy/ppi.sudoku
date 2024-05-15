#pragma once
#ifndef GRACZ_H
#define GRACZ_H

#include <string>

class Gracz {
private:
    std::string nick;
public:
    Gracz(const std::string& nick);
    const std::string& pobierzNick() const;
};
#endif