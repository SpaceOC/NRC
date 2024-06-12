#ifdef _WIN32
    #include <windows.h>
#endif

void FixNOW() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
    #endif
}