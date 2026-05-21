#ifndef NRC_BASE_TUI_SCREENMANAGER_H_
#define NRC_BASE_TUI_SCREENMANAGER_H_

#include "Core/tui/screens/base_screen.h"

namespace core {
    class ScreenManager {
    public:
        ScreenManager();
        ~ScreenManager();

        void changeScreen(BaseScreen* newScreen, bool destroyPrevious = true);
        BaseScreen* getCurrentScreen();
        BaseScreen* getPreviousScreen();

        void drawScreen();

    protected:
        BaseScreen* _currentScreen = nullptr;
        BaseScreen* _previousScreen = nullptr;
    };

    // global ScreenManager
    ScreenManager* screenManager();
};

#endif // NRC_BASE_TUI_SCREENMANAGER_H_