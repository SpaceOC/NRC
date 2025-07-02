#ifndef NRC_BASE_GPRINT_H_
#define NRC_BASE_GPRINT_H_
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <any>
#include <filesystem>
#include "Core/print/print_tools.h"
#include "Core/utils/string_util.h"

#define sgtrace(...)             core::gprint(DEFAULT_PRINT_COLOR, FILENAME + "::" + std::to_string(__LINE__) + ": ", __VA_ARGS__)
#define gtrace(color, ...)       core::gprint(color, FILENAME + "::" + std::to_string(__LINE__) + ": ", __VA_ARGS__)

namespace core {
	std::string gprint();
	std::string gprint(const std::string& message, const PrintColors& color = DEFAULT_PRINT_COLOR);

	template<typename T, typename = is_not_printable_class<T>>
	std::string gprint(const T& message, const PrintColors& color = DEFAULT_PRINT_COLOR) {
		std::string escapeSequence = core::getColorEscapeSequence(color);
		return escapeSequence + string_util::valueToString(message) + "\033[0m";
	}

	template<typename T, typename = is_not_printable_class<T>>
	std::string gprint(const std::vector<T> yourVector, const PrintColors& color = DEFAULT_PRINT_COLOR) {
		std::string escapeSequence = core::getColorEscapeSequence(color);
		std::string temp = escapeSequence + "[ ";
		auto it = yourVector.begin();
		for (const T& str : yourVector) {
			if (it == yourVector.begin())
				temp += string_util::valueToString(str);
			else
				temp += ", " + string_util::valueToString(str);
			it++;
		}
		temp += " ]";
		return temp + "\033[0m";
	}

	template<typename T, typename A, typename = is_not_printable_class<A>, typename = is_not_printable_class<T>>
	std::string gprint(const std::map<T, A> yourMap, const PrintColors& color = DEFAULT_PRINT_COLOR) {
		std::string escapeSequence = core::getColorEscapeSequence(color);
		std::string temp = escapeSequence + "[ ";
		auto it = yourMap.begin();
		for (const auto& str : yourMap) {
			if (it == yourMap.begin())
				temp += "[ '" + string_util::valueToString(str.first) + "': '" + string_util::valueToString(str.second) + "']";
			else
				temp += ", [ '" + string_util::valueToString(str.first) + "': '" + string_util::valueToString(str.second) + "']";
			it++;
		}
		temp += " ]";
		return temp + "\033[0m";
	}

	std::string gprint(PrintableClass &yourClass, const PrintColors& color = DEFAULT_PRINT_COLOR);

	template<typename T, typename ...Arguments>
	std::string gprint(const PrintColors& color, const T& first, const Arguments&... args) {
		std::string output;
		auto a = first; // уберёшь - сломаешь
		output = gprint(a, color); // заменишь 'a' на first - сломаешь
		if (sizeof...(Arguments) > 0) output += gprint(color, args...);
		return output;
	}
}

#endif