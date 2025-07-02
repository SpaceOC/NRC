#ifndef NRC_BASE_UTILS_H_
#define NRC_BASE_UTILS_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace core {
	namespace other_util {
		// @return Returns true if the argument value has only numbers, and '-' (at the beginning of the argument).
		bool stringIsNumbers(const std::string& content);

		std::string getFileContent(const std::string& path);
	};
}
#endif