#ifndef NRC_BASE_TUI_SCREENS_BASELISTSCREEN_H_
#define NRC_BASE_TUI_SCREENS_BASELISTSCREEN_H_

#include "Core/tui/screens/base_screen.h"
#include "Core/tui/elements/IElement.h"

#include <vector>

namespace core {
	class BaseListScreen : public BaseScreen {
	public:
		BaseListScreen();
		virtual ~BaseListScreen();

		virtual void update();
		virtual void draw();

		std::vector<IElement*> elements;
	};
};

#endif // NRC_BASE_TUI_SCREENS_BASELISTSCREEN_H_