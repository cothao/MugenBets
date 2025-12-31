#include <windows.h>
#include "mugenBattleManager.h"

namespace MugenBattleManager
{

inline STARTUPINFOA si{ sizeof(si) };
inline PROCESS_INFORMATION pi1{}, pi2{};

void StartBattle()
{

	SetCurrentDirectoryA("C:/Users/colli/source/repos/Mugen-Bets/Mugen-Bets/mugen-1.1b1");

CreateProcessA(
NULL,
(LPSTR)"MugenWatcher.exe",
NULL, NULL, FALSE, 0, NULL, NULL,
&si, &pi1
);

CreateProcessA(
NULL,
(LPSTR)"mugen.exe -log -p1 Old_Foxy -p1.ai 9 -p2 MK2_KINTARO -p2.ai 9 -p3 kfm -p3.ai 9 -rounds 1",
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



}
