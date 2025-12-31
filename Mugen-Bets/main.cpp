#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include "mugenBattleManager.h"

int main()
{
	MugenBattleManager::StartBattle();
    MugenBattleManager::WaitForBattleEnd();
	MugenBattleManager::GetBattleResult();
	return 0;
}