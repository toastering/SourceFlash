#include "Detour.hpp"
#include "CSignature.h"

typedef void(*CL_Move_t)(float accumulated_extra_samples, bool bFinalTick);
DetourHook cl_move_detour;
int SpeedValue = 4;

void CL_Move_hook(float accumulated_extra_samples, bool bFinalTick) {
	CL_Move_t original = (CL_Move_t)cl_move_detour.GetOriginalFunc();

	if (GetAsyncKeyState(VK_NUMPAD8) & 1) {
		SpeedValue++;
		std::cout << SpeedValue << std::endl;
	}
	if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
		SpeedValue--;
		std::cout << SpeedValue << std::endl;
	}

	if (GetAsyncKeyState(VK_SHIFT)) {
		for (int i = 0; i < SpeedValue; i++)
		{
			original(accumulated_extra_samples, bFinalTick);
		}
	}

	if (!GetAsyncKeyState(VK_INSERT)) {
		original(accumulated_extra_samples, bFinalTick);
	}
	cl_move_detour.RestorePatch();
}

DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	AllocConsole();
	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

	std::cout << "Welcome to SourceFlash! Press your end key to detach." << std::endl;

	DWORD dwcldtmove = gSignatures.GetEngineSignature("55 8B EC 83 EC ? 83 3D ? ? ? ? 02 0F 8C ? ? 00 00 E8 ? ? ? 00 84 C0");
	cl_move_detour.Init(dwcldtmove, (void*)CL_Move_hook);

	while (!GetAsyncKeyState(VK_END)) Sleep(1);

	cl_move_detour.Shutdown();
	std::cout << "Detaching. Bye bye!\nIt is now safe to re-inject SourceFlash." << std::endl;
	FreeLibraryAndExitThread(static_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dwMainThread, hInstance, 0, 0));
	}
	return true;
}