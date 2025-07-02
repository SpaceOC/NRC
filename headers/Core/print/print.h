#ifndef NRC_BASE_PRINT_H_
#define NRC_BASE_PRINT_H_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <any>
#include "Core/print/print_tools.h"
#include "Core/utils/string_util.h"

#define strace(...)             core::print(DEFAULT_PRINT_COLOR, FILENAME + "::", __LINE__, "::[" + std::string(__FUNCTION__) + "]: ", __VA_ARGS__); core::print()
#define trace(color, ...)       core::print(color, FILENAME + "::", __LINE__, "::[" + std::string(__FUNCTION__) + "]: ", __VA_ARGS__); core::print()

namespace core {
	void print();
	void print(const std::string& message, const PrintColors& color = DEFAULT_PRINT_COLOR);
	template<typename T, typename = is_not_printable_class<T>>
	void print(const T& message, const PrintColors& color = DEFAULT_PRINT_COLOR) {
		std::string escapeSequence = core::getColorEscapeSequence(color);
		std::cout << escapeSequence << message << "\033[0m";
	}
	template<typename T, typename = is_not_printable_class<T>>
	void print(const std::vector<T> yourVector, const PrintColors& color = DEFAULT_PRINT_COLOR) {
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
		std::cout << temp << "\033[0m";
	}
	template<typename T, typename A, typename = is_not_printable_class<A>, typename = is_not_printable_class<T>>
	void print(const std::map<T, A> yourMap, const PrintColors& color = DEFAULT_PRINT_COLOR) {
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
		std::cout << temp << "\033[0m";
	}
	void print(PrintableClass &yourClass, const PrintColors& color = DEFAULT_PRINT_COLOR);

	template<typename T, typename ...Arguments>
	void print(const PrintColors& color, const T& first, const Arguments&... args) {
		auto a = first; // уберёшь - сломаешь
		print(a, color); // заменишь 'a' на first - сломаешь
		if (sizeof...(Arguments) > 0) print(color, args...);
	}
}

#endif