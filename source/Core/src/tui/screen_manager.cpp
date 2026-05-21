#include "Core/tui/screen_manager.h"

core::ScreenManager globalScreenManager;

core::ScreenManager* core::screenManager() {
    return &globalScreenManager;
}

core::ScreenManager::ScreenManager() {}

core::ScreenManager::~ScreenManager() {
    if (_currentScreen)
        delete _currentScreen;
    if (_previousScreen)
        delete _previousScreen;
}

void core::ScreenManager::changeScreen(core::BaseScreen* newScreen, bool destroyPrevious) {
    if (destroyPrevious) {
        if (_currentScreen)
            delete _currentScreen;
        _currentScreen = nullptr;
    }
    else {
        if (_previousScreen)
            delete _previousScreen;

        _previousScreen = _currentScreen;
    }

    _currentScreen = newScreen;
}

core::BaseScreen* core::ScreenManager::getCurrentScreen() {
    return _currentScreen;
}

core::BaseScreen* core::ScreenManager::getPreviousScreen() {
    return _previousScreen;
}

void core::ScreenManager::drawScreen() {
    if (_currentScreen)
        _currentScreen->draw();
}