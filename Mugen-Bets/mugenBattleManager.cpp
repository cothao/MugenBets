#include <windows.h>
#include "mugenBattleManager.h"
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;
using characterPointer = std::shared_ptr<Character>;

namespace MugenBattleManager
{

inline STARTUPINFOA si{ sizeof(si) };
inline PROCESS_INFORMATION pi1{}, pi2{};

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

void StartBattle()
{


	SetCurrentDirectoryA("C:/Users/colli/source/repos/Mugen-Bets/Mugen-Bets/mugen-1.1b1");

	GetCharacters();

	characters.push_back(characterPointer(new Character("Old_Foxy", "1")));
	characters.push_back(characterPointer(new Character("MK2_KINTARO", "1")));

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
	
void WaitForBattleEnd()
{
	
	WaitForSingleObject(pi1.hProcess, INFINITE);
	
	CloseHandle(pi1.hProcess);
	CloseHandle(pi1.hThread);

}

void GetCharacters()
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
				
				availableCharacters.push_back(finalCharacterName);

			}
		}

	}

}

}
