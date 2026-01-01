#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include "mugenBattleManager.h"

int main()
{
	while (true)
	{
		MugenBattleManager::StartBattle();
		MugenBattleManager::WaitForBattleEnd();
		MugenBattleManager::SetMatchResult();
		MugenBattleManager::SetCharactersStatsJSON();

		std::chrono::duration<double, std::milli> time{ 10000 };
		std::this_thread::sleep_for(time);

	}
	return 0;
}