#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include "mugenBattleManager.h"


template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

int main()
{
	MugenBattleManager::StartBattle();
    MugenBattleManager::WaitForBattleEnd();
	return 0;
}