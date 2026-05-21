#include "Core/tui/screens/user_manager_screen.h"

#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>

#include "Core/users/user_manager.h"
#include "Core/users/user.h"

core::UserManagerScreen::UserManagerScreen() {}
core::UserManagerScreen::~UserManagerScreen() {}

void core::UserManagerScreen::update() {}

void core::UserManagerScreen::draw() {
	if (!core::userManager())
		return;

	std::map<std::string, std::string> userMap = core::userManager()->getUserMap();
	if (userMap.empty())
		return;

	std::vector<std::vector<std::string>> strVector = {{"Name", "Display Name", "Language", "Permissions"}};

	const size_t size = userMap.size();
	char* colors = new char[size];

	size_t colorIndex = 0;
	for (const auto& pair : userMap) {
		core::User* user = &(core::userManager()->getUserData(pair.first));
		std::string username = user->getUsername();
		std::string displayName = user->getDisplayName();
		if (displayName == username)
			displayName = "";
		
		std::string language = user->getLanguage();
		std::string permissions = core::userPermissionsS(user->getPermissions());

		std::vector<std::string> strData = {username, displayName, language, permissions};
		strVector.push_back(strData);

		core::UserPermissions perms = user->getPermissions();

		colors[colorIndex++] = perms < core::UserPermissions::ADMIN ? 0 : (perms == core::UserPermissions::ROOT ? 2 : 1);
	}

	auto table = ftxui::Table(strVector);
	table.SelectAll().Border(ftxui::LIGHT);
	table.SelectColumn(0).Border(ftxui::LIGHT);
	table.SelectColumn(1).Border(ftxui::LIGHT);
	table.SelectColumn(2).Border(ftxui::LIGHT);
	table.SelectColumn(3).Border(ftxui::LIGHT);

	table.SelectRow(0).Decorate(ftxui::bold);
	table.SelectRow(0).SeparatorVertical(ftxui::LIGHT);
	table.SelectRow(0).Border(ftxui::DOUBLE);

	table.SelectColumn(2).DecorateCells(ftxui::align_right);

	for (size_t i = 0; i < size; i++) {
		char color = colors[i];
		if (color == 0)
			table.SelectCell(3, i+1).Decorate(ftxui::color(ftxui::Color::Green));
		else if (color == 1)
			table.SelectCell(3, i+1).Decorate(ftxui::color(ftxui::Color::Blue));
		else
			table.SelectCell(3, i+1).Decorate(ftxui::color(ftxui::Color::Red));
	}

	delete[] colors;

	auto document =
		ftxui::vbox({
			ftxui::text("You are " + core::userManager()->getYourUsername() + "\n"),
			ftxui::text("User Manager Info \\/\n"),
			table.Render()
		});

	auto screen = ftxui::Screen::Create(ftxui::Dimension::Fit(document, true));
	ftxui::Render(screen, document);
	screen.Print();
}