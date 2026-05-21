#include "Core/tui/elements/ListElement.h"

core::ListElement::ListElement() {}

core::ListElement::~ListElement() {
    for (const auto& e : elements)
        delete e;
}

ftxui::Element core::ListElement::build() {
    std::vector<ftxui::Element> e;
    for (const auto& i : elements) {
        if (i->canBuild())
            e.push_back(i->build());
    }

    if (vertically)
        return ftxui::vbox(e);
    return ftxui::hbox(e);
}

bool core::ListElement::canBuild() {
    return !elements.empty();
}