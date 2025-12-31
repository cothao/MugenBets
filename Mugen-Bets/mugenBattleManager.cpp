#include <windows.h>
#include "mugenBattleManager.h"
#include <memory>
#include <filesystem>
#include <random>
#include <set>
#include "helpers.h"
namespace fs = std::filesystem;
using characterPointer = std::shared_ptr<Character>;

namespace MugenBattleManager
{
	STARTUPINFOA si{sizeof(si)};
	PROCESS_INFORMATION pi1{}, pi2{};
	std::string startFlag{ "" };
};
	/*
	std::map <std::string, std::string> startFlags =
	{
		{"-p1", nullptr},
		{"-p2", nullptr},
		{"-p3", nullptr},
		{"-p4", nullptr},
		{"-rounds", nullptr},
		{"-stage", nullptr},
	};
	*/
	
void MugenBattleManager::StartBattle()
{


	SetCurrentDirectoryA("C:/Users/colli/source/repos/Mugen-Bets/Mugen-Bets/mugen-1.1b1");

	if (availableCharacters.empty()) GetCharacters();

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
	
void MugenBattleManager::GetCharacters()
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

void MugenBattleManager::GetBattleResult()
{

	std::vector<std::string> lines{};

	std::ifstream file("MugenWatcher.log");

	std::string line;

	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	std::vector<std::string> matchResult = splitString(lines.back(), ',');

	std::cout << matchResult[2] << '\n';
	std::cout << matchResult[3] << '\n';

	std::cout << "MATCH RESULT: " << charactersForBattle[0]->characterName << " " << matchResult[2] << " | " << charactersForBattle[1]->characterName << " " << matchResult[3] << '\n';

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

	while (randomCharacterIndexes.size() < characterTeamSizes)
	{
		uint32_t randomNumber = dist(rng);
		randomCharacterIndexes.insert(randomNumber);

		if (randomCharacterIndexes.find(randomNumber) != randomCharacterIndexes.end())
		{
			charactersForBattle.push_back(characterPointer(new Character(availableCharacters.at(randomNumber), "1")));
		}

	}

}
