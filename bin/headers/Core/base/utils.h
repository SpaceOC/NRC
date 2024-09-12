#ifndef NRC_BASE_UTILS_H_
#define NRC_BASE_UTILS_H_
#include <string>
#include <vector>

namespace core {
    class Utils {
        public:
            Utils();
            static bool stringIsNumbers(const std::string& content);
            static std::vector<std::string> split(const std::string& content, const char what);
    };
}
#endif