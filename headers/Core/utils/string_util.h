#ifndef NRC_BASE_STRING_UTILS_H_
#define NRC_BASE_STRING_UTILS_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace core {
	namespace string_util {
		/* 
		 * Splits the content value into parts.
		 * @param content The text to be further divided and returned as a vector.
		 * @param what Splitter.
		 */
		std::vector<std::string> split(const std::string& content, const char what);

		std::string replace(std::string& content, const std::string& a, const std::string& b);

		bool endsWith(const std::string& str, const std::string& str2);

		template <typename T>
		std::string valueToString(const T &value) {
			std::ostringstream oss;
			oss << value;
			return oss.str();
		}

		template<typename T>
		std::string vectorToString(const std::vector<T>& value) {
			std::string str = "[ ";
			auto it = value.begin();
			for (const T& a : value) {
				if (it == value.begin())
					str += string_util::valueToString(a);
				else
					str += ", " + string_util::valueToString(a);
				it++;
			}
			str += " ]";
			return str;
		}

		template<typename A, typename B>
		std::string mapToString(const std::map<A, B>& value) {
			std::string str = "[ ";
			auto it = value.begin();
			for (const auto& a : value) {
				if (it == value.begin())
					str += "[ '" + string_util::valueToString(a.first) + "': '" + string_util::valueToString(a.second) + "']";
				else
					str += ", [ '" + string_util::valueToString(a.first) + "': '" + string_util::valueToString(a.second) + "']";
				it++;
			}
			str += " ]";
			return str;
		}
	};
}
#endif