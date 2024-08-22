#pragma once
#include <string>
#include <vector>

namespace core {
    class Utils {
        public:
            static bool existsInVector(const std::vector<std::string>& vector, const std::string& var);
            template<typename T>
            static bool existsInVector(const std::vector<T>& vector, const T& content);
            static bool stringIsNumbers(const std::string& content);
    };
}