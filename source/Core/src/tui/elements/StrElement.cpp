#include "Core/tui/elements/StrElement.h"

core::StrElement::StrElement() {}
core::StrElement::~StrElement() {}

ftxui::Element core::StrElement::build() {
    return ftxui::text(text);
}

bool core::StrElement::canBuild() {
    return !text.empty();
}