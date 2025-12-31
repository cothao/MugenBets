#ifndef MUGENBATTLEMANAGER_H

#define MUGENBATTLEMANAGER_H

#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include "character.h"

namespace MugenBattleManager
{

	void StartBattle();
	void WaitForBattleEnd();
	void GetCharacters();
	void GetBattleResult();
	void SetStartFlag();
	void SetCharactersForBattle();
	static std::vector<std::shared_ptr<Character>> charactersForBattle;
	static std::vector <std::string> availableCharacters;
	extern STARTUPINFOA si;
	extern PROCESS_INFORMATION pi1;
	extern PROCESS_INFORMATION pi2;
	extern std::string startFlag;

}

#endif 