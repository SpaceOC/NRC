#ifndef NRC_BASE_TUI_ELEMENTS_LISTELEMENT_H_
#define NRC_BASE_TUI_ELEMENTS_LISTELEMENT_H_

#include "Core/tui/elements/IElement.h"

#include <vector>

namespace core {
	class ListElement : public IElement {
	public:
		ListElement();
		virtual ~ListElement();

		std::vector<IElement*> elements;
		bool vertically = true;

		virtual ftxui::Element build();
		virtual bool canBuild();
	};
};

#endif // NRC_BASE_TUI_ELEMENTS_LISTELEMENT_H_