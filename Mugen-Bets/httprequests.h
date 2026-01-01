#ifndef HTTPREQUESTS_H
#define HTTPREQUESTS_H

#include <windows.h>
#include <winhttp.h>
#include <string>

#pragma comment(lib, "winhttp.lib")

namespace HTTP
{
	void fetch(const wchar_t* endpoint, const wchar_t* requestType, std::string payload);
}

#endif