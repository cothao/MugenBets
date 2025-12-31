#include <windows.h>
#include "mugenBattleManager.h"
#include <memory>
#include <filesystem>
#include <random>
#include "helpers.h"

namespace fs = std::filesystem;
using characterPointer = std::shared_ptr<Character>;

namespace MugenBattleManager
{
	STARTUPINFOA si{sizeof(si)};
	PROCESS_INFORMATION pi1{}, pi2{};
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

	// Eventually we want to move these lines to their own separate function.. but not now

	characters = {};

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, availableCharacters.size());

	for (int characterIndex = 0; characterIndex < 2; characterIndex++)
	{
		
		characters.push_back(characterPointer(new Character(availableCharacters[dist(rng)], "1")));
	
	}

	std::string startFlag = "mugen.exe";
	startFlag += " -p1 ";
	startFlag += characters[0]->characterName;
	startFlag += " -p1.color ";
	startFlag += characters[0]->colorPalette;
	startFlag += " -p1.ai 9 ";
	startFlag += " -p2 ";
	startFlag += characters[1]->characterName;
	startFlag += " -p2.color ";
	startFlag += characters[1]->colorPalette;
	startFlag += " -p2.ai 9 ";
	startFlag += "-rounds 1";

	std::cout << startFlag << '\n';

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

			std::string filePath = fs::absolute(directoryContents.path()).string();

			if (filePath.find(".def") != std::string::npos && 
				filePath.find("ending") == std::string::npos && 
				filePath.find("intro") == std::string::npos && 
				filePath.find("CONFIG") == std::string::npos)
			{
				size_t pathIndex = filePath.find_last_of("\\");
				std::string fileName(filePath.substr(pathIndex + 1, filePath.find(".def")));
				std::string finalCharacterName = fileName.substr(0, fileName.find_last_of("."));
				std::cout << finalCharacterName << '\n';
				availableCharacters.push_back(finalCharacterName);

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

	std::cout << "MATCH RESULT: " << characters[0]->characterName << " " << matchResult[2] << " | " << characters[1]->characterName << " " << matchResult[3] << '\n';

}
