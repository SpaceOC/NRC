#include "Core/print/gprint.h"

std::string core::gprint() {
	return "";
}

std::string core::gprint(const std::string& message, const PrintColors& color) {
	std::string escapeSequence = core::getColorEscapeSequence(color);
	return escapeSequence + message + "\033[0m";
}

std::string core::gprint(PrintableClass &yourClass, const PrintColors& color) {
	std::string escapeSequence = core::getColorEscapeSequence(color);
	return escapeSequence + yourClass.toString() + "\033[0m";
}