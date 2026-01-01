#include <windows.h>
#include "mugenBattleManager.h"
#include <memory>
#include <filesystem>
#include <random>
#include <set>
#include <nlohmann/json.hpp>
#include "helpers.h"
#include "httprequests.h"

using json = nlohmann::json;
using characterPointer = std::shared_ptr<Character>;

namespace fs = std::filesystem;

namespace MugenBattleManager
{
	STARTUPINFOA si{sizeof(si)};
	PROCESS_INFORMATION pi1{}, pi2{};
	MATCH_TYPE matchType{TWO_VERSUS_ONE};
	std::string startFlag{ "" };
	std::map<std::string, std::map<std::string, std::string>> matchResult{
		{"Winners", {}},
		{"Losers", {}},
	};
};
	
void MugenBattleManager::StartBattle()
{

	SetCurrentDirectoryA("C:/Users/colli/source/repos/Mugen-Bets/Mugen-Bets/mugen-1.1b1");

	if (availableCharacters.empty()) PopulateAvailableCharacters();

	SetCharactersForBattle();
	
	SetStartFlag();

	CreateProcessA(
	NULL,
	(LPSTR)"MugenWatcher.exe",
	NULL, NULL, FALSE, 0, NULL, NULL,
	&si, &pi1
	);
	
	CreateProcessA(
	NULL,
	(LPSTR)startFlag.c_str(),
	NULL, NULL, FALSE, 0, NULL, NULL,
	&si, &pi1
	);
	

}
		
void MugenBattleManager::WaitForBattleEnd()
{
		
	WaitForSingleObject(pi1.hProcess, INFINITE);
		
	CloseHandle(pi1.hProcess);
	CloseHandle(pi1.hThread);
	
}
	
void MugenBattleManager::PopulateAvailableCharacters()
{
	
	std::string path = "C:/Users/colli/source/repos/Mugen-Bets/Mugen-Bets/mugen-1.1b1/chars/";
	for (const auto& characterDirectory : fs::directory_iterator(path))
	{

		for (const auto& directoryContents : fs::directory_iterator(characterDirectory.path()))
		{

			fs::path filePath(directoryContents.path());

			if (filePath.extension() == ".def" &&
				filePath.stem().string() != "ending" &&
				filePath.stem().string() != "intro" &&
				filePath.stem().string() != "CONFIG")
			{
				
				std::string characterName = filePath.stem().string();
				availableCharacters.push_back(characterName);

			}
		}

	}


}

std::vector<std::string> MugenBattleManager::GetBattleResult()
{

	std::vector<std::string> lines{};

	std::ifstream file("MugenWatcher.log");

	std::string line;

	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	return lines;

}

void MugenBattleManager::SetStartFlag()
{

	switch (charactersForBattle.size())
	{


	case 2:
		startFlag = "mugen.exe";
		startFlag += " -p1 ";
		startFlag += charactersForBattle[0]->characterName;
		startFlag += " -p1.color ";
		startFlag += charactersForBattle[0]->colorPalette;
		startFlag += " -p1.ai 9 ";
		startFlag += " -p2 ";
		startFlag += charactersForBattle[1]->characterName;
		startFlag += " -p2.color ";
		startFlag += charactersForBattle[1]->colorPalette;
		startFlag += " -p2.ai 9 ";
		startFlag += "-rounds 1";
		std::cout << startFlag << '\n';
		break;
	case 3:
		startFlag = "mugen.exe";
		startFlag += " -p1 ";
		startFlag += charactersForBattle[0]->characterName;
		startFlag += " -p1.color ";
		startFlag += charactersForBattle[0]->colorPalette;
		startFlag += " -p1.ai 9 ";
		startFlag += " -p2 ";
		startFlag += charactersForBattle[1]->characterName;
		startFlag += " -p2.color ";
		startFlag += charactersForBattle[1]->colorPalette;
		startFlag += " -p2.ai 9 ";
		startFlag += " -p3 ";
		startFlag += charactersForBattle[2]->characterName;
		startFlag += " -p3.color ";
		startFlag += charactersForBattle[2]->colorPalette;
		startFlag += " -p3.ai 9 ";
		startFlag += "-rounds 1";
		std::cout << startFlag << '\n';
		break;
	case 4:
		startFlag = "mugen.exe";
		startFlag += " -p1 ";
		startFlag += charactersForBattle[0]->characterName;
		startFlag += " -p1.color ";
		startFlag += charactersForBattle[0]->colorPalette;
		startFlag += " -p1.ai 9 ";
		startFlag += " -p2 ";
		startFlag += charactersForBattle[1]->characterName;
		startFlag += " -p2.color ";
		startFlag += charactersForBattle[1]->colorPalette;
		startFlag += " -p2.ai 9 ";
		startFlag += " -p3 ";
		startFlag += charactersForBattle[2]->characterName;
		startFlag += " -p3.color ";
		startFlag += charactersForBattle[2]->colorPalette;
		startFlag += " -p3.ai 9 ";
		startFlag += " -p4 ";
		startFlag += charactersForBattle[3]->characterName;
		startFlag += " -p4.color ";
		startFlag += charactersForBattle[3]->colorPalette;
		startFlag += " -p4.ai 9 ";
		startFlag += "-rounds 1";
		std::cout << startFlag << '\n';
		break;
	}

}

void MugenBattleManager::SetCharactersForBattle()
{
	
	charactersForBattle = {};

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, availableCharacters.size() - 1);
	std::uniform_int_distribution<std::mt19937::result_type> sizeDist(2, 4);
	std::set<int> randomCharacterIndexes{};
	uint32_t characterTeamSizes = sizeDist(rng);
	uint32_t previousSetSize = randomCharacterIndexes.size();

	while (randomCharacterIndexes.size() < matchType)
	{
		uint32_t randomNumber = dist(rng);
		randomCharacterIndexes.insert(randomNumber);

		if (randomCharacterIndexes.size() != previousSetSize) // ensuring we add a unique character and not a dupe
		{
			charactersForBattle.push_back(characterPointer(new Character(availableCharacters.at(randomNumber), "1")));
			previousSetSize = randomCharacterIndexes.size();
		}


	}

}

void MugenBattleManager::SetCharactersStatsJSON()
{

	std::ifstream f("characterStats.json");
	json characterData = json::parse(f);
	json jsonData;

	for (std::map<std::string, std::string>::iterator character = matchResult["Winners"].begin(); character != matchResult["Winners"].end(); ++character)
	{
		

		std::string characterName = character->first;
		std::cout << "Character: " << characterName << '\n';

		if (!characterData["characters"].contains(characterName))
		{

			std::cout << characterName << '\n';
			std::cout << "Score: " << std::stoi(matchResult["Winners"][characterName]) << '\n';
			int score = std::stoi(matchResult["Winners"][characterName]);

			json playerMatchData =
			{
				{"CharacterName", characterName},
				{"Wins", score},
				{"Losses", 0},
				{"WinRate", 0.0},
				{"Matches", 1},
				{"Tier", "N/A"}
			};

			characterData["characters"][characterName] = playerMatchData;
			jsonData[characterName] = playerMatchData;

		}
		else
		{
			std::cout << "Score: " << std::stoi(matchResult["Winners"][characterName])<< '\n';
			int score = std::stoi(matchResult["Winners"][characterName]);
	
			int updatedWins = characterData["characters"][characterName]["Wins"] + score;
			int updatedLosses = characterData["characters"][characterName]["Losses"];
			int updatedMatches = characterData["characters"][characterName]["Matches"] + 1;
			double updatedWinRate = updatedMatches > 10 ? (double)updatedWins/(double)updatedMatches : 0;
			std::string tier = GetTier(updatedWinRate);

			std::cout << "Updated Win Rate for " << characterName << ": " << updatedWinRate << '\n';
			
			json currentCharacterStats =
			{
				{"CharacterName", characterName},
				{"Wins", updatedWins},
				{"Losses", updatedLosses},
				{"WinRate", updatedWinRate},
				{"Matches", updatedMatches},
				{"Tier", tier}
			};

			characterData["characters"][characterName].clear();
			jsonData[characterName] = currentCharacterStats;
			characterData["characters"][characterName] = currentCharacterStats;
		}

	}
	
	for (std::map<std::string, std::string>::iterator character = matchResult["Losers"].begin(); character != matchResult["Losers"].end(); ++character)
	{

		std::string characterName = character->first;

		if (!characterData["characters"].contains(characterName))
		{
			

			std::cout << characterName << '\n';

			std::cout << "Score: " << std::stoi(matchResult["Losers"][characterName]) << '\n';
			int score = std::stoi(matchResult["Losers"][characterName]);

			json playerMatchData =
			{
				{"CharacterName", characterName},
				{"Wins", 0},
				{"Losses", score},
				{"WinRate", 0.0},
				{"Matches", 1},
				{"Tier", "N/A"}
			};

			jsonData[characterName] = playerMatchData;
			characterData["characters"][characterName] = playerMatchData;
	
		}
		else
		{
			std::cout << "Score: " << std::stoi(matchResult["Losers"][characterName]) << '\n';
			int score = std::stoi(matchResult["Losers"][characterName]);

			int updatedWins = characterData["characters"][characterName]["Wins"];
			int updatedLosses = characterData["characters"][characterName]["Losses"] + score;
			int updatedMatches = characterData["characters"][characterName]["Matches"] + 1;
			double updatedWinRate = updatedMatches > 10 ? (double)updatedWins/(double)updatedMatches : 0;
			std::string tier = GetTier(updatedWinRate);

			json currentCharacterStats =
			{
				{"CharacterName", characterName},
				{"Wins", updatedWins},
				{"Losses", updatedLosses},
				{"WinRate", updatedWinRate},
				{"Matches", updatedMatches},
				{"Tier", tier}
			};

			characterData["characters"][characterName].clear();
			jsonData[characterName] = currentCharacterStats;
			characterData["characters"][characterName] = currentCharacterStats;
		}
	}

	const wchar_t* requestUrl = L"8xkovn5om1.execute-api.us-east-1.amazonaws.com";
	const wchar_t* requestType = L"GET";

	std::cout << jsonData.dump() << '\n';
	HTTP::fetch(requestUrl, requestType, jsonData.dump());
	std::ofstream out("characterStats.json");
	out << characterData.dump();


}

void MugenBattleManager::SetMatchResult()
{
	matchResult["Winners"].clear();
	matchResult["Losers"].clear();
	std::vector<std::string> logLines = GetBattleResult();

	std::vector<std::string> matchResultInfo = splitString(logLines.back(), ',');
	
	switch (matchType)
	{
	case ONE_VERSUS_ONE:

		if (matchResultInfo[2] == "1")
		{
			matchResult["Winners"][charactersForBattle[0]->characterName] = "1";

			matchResult["Losers"][charactersForBattle[1]->characterName] = "1";
		}
			
		break;
	case TWO_VERSUS_ONE:

		if (matchResultInfo[2] == "1")
		{
			matchResult["Winners"][charactersForBattle[0]->characterName] = "1";
			matchResult["Winners"][charactersForBattle[2]->characterName] = "1";
			
			matchResult["Losers"][charactersForBattle[1]->characterName] = "1";
		}
		else
		{
			matchResult["Winners"][charactersForBattle[1]->characterName] = "1";

			matchResult["Losers"][charactersForBattle[0]->characterName] = "1";
			matchResult["Losers"][charactersForBattle[2]->characterName] = "1";
		}
		break;

	case TWO_VERSUS_TWO:

		if (matchResultInfo[2] == "1")
		{
			matchResult["Winners"][charactersForBattle[0]->characterName] = "1";
			matchResult["Winners"][charactersForBattle[2]->characterName] = "1";

			matchResult["Losers"][charactersForBattle[1]->characterName] = "1";
			matchResult["Losers"][charactersForBattle[3]->characterName] = "1";
		}
		else
		{
			matchResult["Winners"][charactersForBattle[1]->characterName] = "1";
			matchResult["Winners"][charactersForBattle[3]->characterName] = "1";
			
			matchResult["Losers"][charactersForBattle[0]->characterName] = "1";
			matchResult["Losers"][charactersForBattle[2]->characterName] = "1";
		}
		break;
	}

}

std::string MugenBattleManager::GetTier(double winRate)
{

	winRate = winRate * 100;

	if (winRate > 96)
	{
		return "SSS";
	}
	else if (winRate > 93)
	{
		return "SS";
	}
	else if (winRate > 90)
	{
		return "S";
	}
	else if (winRate > 86)
	{
		return "A+";
	}
	else if (winRate > 83)
	{
		return "A";
	}
	else if (winRate > 80)
	{
		return "A-";
	}
	else if (winRate > 76)
	{
		return "B+";
	}
	else if (winRate > 73)
	{
		return "B";
	}

	else if (winRate > 70)
	{
		return "B-";
	}

	else if (winRate > 66)
	{
		return "C+";
	}

	else if (winRate > 63)
	{
		return "C";
	}

	else if (winRate > 60)
	{
		return "C-";
	}

	else if (winRate > 56)
	{
		return "D+";
	}

	else if (winRate > 53)
	{
		return "D";
	}

	else if (winRate > 50)
	{
		return "D-";
	}
	else
	{
		return "F";
	}

}
