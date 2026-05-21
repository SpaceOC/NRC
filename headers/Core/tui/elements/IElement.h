#ifndef NRC_BASE_TUI_ELEMENTS_IELEMENT_H_
#define NRC_BASE_TUI_ELEMENTS_IELEMENT_H_

#include <ftxui/dom/elements.hpp>

namespace core {
	class IElement {
	public:
		inline IElement() {}
		virtual ~IElement() {}

		virtual ftxui::Element build() = 0;
		virtual bool canBuild() = 0;
	};
};

#endif // NRC_BASE_TUI_ELEMENTS_IELEMENT_H_