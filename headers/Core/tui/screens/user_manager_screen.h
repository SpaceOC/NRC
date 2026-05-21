#ifndef NRC_BASE_TUI_SCREENS_USERMANAGERSCREEN_H_
#define NRC_BASE_TUI_SCREENS_USERMANAGERSCREEN_H_

#include "Core/tui/screens/base_screen.h"

namespace core {
	class UserManagerScreen : public BaseScreen {
	public:
		UserManagerScreen();
		virtual ~UserManagerScreen();

		virtual void update();
		virtual void draw();
	};
};

#endif // NRC_BASE_TUI_SCREENS_USERMANAGERSCREEN_H_