#ifndef NRC_BASE_TUI_SCREENS_BASESCREEN_H_
#define NRC_BASE_TUI_SCREENS_BASESCREEN_H_

namespace core {
	class BaseScreen {
	public:
		inline BaseScreen() {}
		virtual ~BaseScreen() {}

		virtual void update() = 0;
		virtual void draw() = 0;
	};
};

#endif // NRC_BASE_TUI_SCREENS_BASESCREEN_H_