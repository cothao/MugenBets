#ifndef CHARACTER_H

#define CHARACTER_H

#include <string>

class Character
{

public:
	
	Character(std::string characterName, std::string colorPalette);

	std::string			colorPalette;
	std::string			characterName;

};

#endif