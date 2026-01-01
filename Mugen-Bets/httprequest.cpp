#include "httprequests.h"
#include <iostream>
#include <vector>
#pragma comment(lib, "winhttp.lib")

void HTTP::fetch(const wchar_t* endpoint, const wchar_t* requestType, std::string payload)
{


    HINTERNET session = WinHttpOpen(L"StatsClient",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);

    if (!session)
    {
        std::cerr << "WinHttpOpen failed\n";
        return;
    }

    HINTERNET connect = WinHttpConnect(
        session, endpoint,
        INTERNET_DEFAULT_HTTPS_PORT, 0);

    HINTERNET request = WinHttpOpenRequest(
        connect, requestType, L"/dev",
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    const char* json = payload.c_str();

    WinHttpSendRequest(
        request,
        L"Content-Type: application/json\r\n",
        -1,
        (LPVOID)json,
        strlen(json),
        strlen(json),
        0);

    if (WinHttpReceiveResponse(request, NULL) == FALSE)
    {
        std::cerr << "Failed to receive response!" << '\n';
    };

    DWORD bytesAvailable = 0;
    do
    {
        WinHttpQueryDataAvailable(request, &bytesAvailable);
        if (bytesAvailable == 0)
            break;

        std::vector<char> buffer(bytesAvailable + 1);
        DWORD bytesRead = 0;

        WinHttpReadData(
            request,
            buffer.data(),
            bytesAvailable,
            &bytesRead);

        buffer[bytesRead] = '\0';
        std::cout << buffer.data();

    } while (bytesAvailable > 0);

    std::cout << std::endl;

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);


}