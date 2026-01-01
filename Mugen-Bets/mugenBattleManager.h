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

enum MATCH_TYPE
{
	ONE_VERSUS_ONE = 2,
	TWO_VERSUS_ONE,
	TWO_VERSUS_TWO
};

namespace MugenBattleManager
{

	void StartBattle();
	void WaitForBattleEnd();
	void PopulateAvailableCharacters();
	std::vector<std::string> GetBattleResult();
	void SetStartFlag();
	void SetCharactersForBattle();
	void SetCharactersStatsJSON();
	void SetMatchResult();
	std::string GetTier(double winRate);
	static std::vector<std::shared_ptr<Character>>	charactersForBattle;
	static std::vector <std::string>				availableCharacters;
	extern std::map < std::string, std::map<std::string, std::string> > matchResult;
	extern MATCH_TYPE								matchType;
	extern STARTUPINFOA								si;
	extern PROCESS_INFORMATION						pi1;
	extern PROCESS_INFORMATION						pi2;
	extern std::string								startFlag;

}

#endif 