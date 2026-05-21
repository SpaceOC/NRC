#include "Core/tui/screens/user_info_screen.h"

#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>

core::UserInfoScreen::UserInfoScreen() {}
core::UserInfoScreen::~UserInfoScreen() {}

void core::UserInfoScreen::update() {}

void core::UserInfoScreen::draw() {
	if (!user)
		return;

	std::string username = user->getUsername();
	std::string displayName = user->getDisplayName();
	std::string language = user->getLanguage();
	std::string permissions = core::userPermissionsS(user->getPermissions());

	std::string name = username;
	if (!displayName.empty() && displayName != username)
		name += " [ " + displayName + " ]";

	auto color =
		user->getPermissions() < core::UserPermissions::ADMIN
			? ftxui::color(ftxui::Color::Green)
			: (user->getPermissions() == core::UserPermissions::ROOT 
				? ftxui::color(ftxui::Color::Red)
				: ftxui::color(ftxui::Color::Blue));
	
	auto document = 
		ftxui::vbox({
			ftxui::text("Username [Display Name]: " + name + "\n"),
			ftxui::text("Language: " + language + "\n"),
			ftxui::text("Permissions: "), ftxui::text(permissions) | color
		});

	auto screen = ftxui::Screen::Create(ftxui::Dimension::Full());
	ftxui::Render(screen, document);
	screen.Print();
}