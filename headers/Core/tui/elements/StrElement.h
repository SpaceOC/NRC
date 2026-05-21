#ifndef NRC_BASE_TUI_ELEMENTS_STRELEMENT_H_
#define NRC_BASE_TUI_ELEMENTS_STRELEMENT_H_

#include "Core/tui/elements/IElement.h"

#include <string>

namespace core {
	class StrElement : public IElement {
	public:
		StrElement();
		virtual ~StrElement();

		std::string text = "";

		virtual ftxui::Element build();
		virtual bool canBuild();
	};
};

#endif // NRC_BASE_TUI_ELEMENTS_STRELEMENT_H_