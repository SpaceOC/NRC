#include "Core/tui/screens/base_list_screen.h"

#include <ftxui/screen/screen.hpp>
#include <memory>

core::BaseListScreen::BaseListScreen() {}
core::BaseListScreen::~BaseListScreen() {}

void core::BaseListScreen::update() {}

void core::BaseListScreen::draw() {
	std::vector<ftxui::Element> tuiElements;
	for (const auto& i : elements) {
		if (i->canBuild())
			tuiElements.push_back(i->build());
	}

	if (!tuiElements.empty()) {
		auto document = ftxui::vbox(tuiElements);

		auto screen = ftxui::Screen::Create(ftxui::Dimension::Full());
		ftxui::Render(screen, document);
		screen.Print();
	}
}