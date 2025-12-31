#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <sstream>
#include <iterator>

template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> splitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


#endif // !HELPERS_H
