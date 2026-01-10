#ifdef _WIN32
#include <Windows.h>
#endif
#include "Core/main.h"

void core::Main::fixNOW() {
	#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	#endif
}