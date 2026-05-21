#ifndef NRC_BASE_TUI_SCREENS_USERINFOSCREEN_H_
#define NRC_BASE_TUI_SCREENS_USERINFOSCREEN_H_

#include "Core/tui/screens/base_screen.h"
#include "Core/users/user.h"

namespace core {
	class UserInfoScreen : public BaseScreen {
	public:
		UserInfoScreen();
		virtual ~UserInfoScreen();

		core::User* user = nullptr;

		virtual void update();
		virtual void draw();
	};
};

#endif // NRC_BASE_TUI_SCREENS_USERINFOSCREEN_H_