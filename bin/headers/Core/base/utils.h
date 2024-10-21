#ifndef NRC_BASE_UTILS_H_
#define NRC_BASE_UTILS_H_
#include <string>
#include <vector>

namespace core {
    class Utils {
        public:
            Utils();

            // @return Returns true if the argument value has only numbers, and '-' (at the beginning of the argument).
            static bool stringIsNumbers(const std::string& content);

            /* 
             * Splits the content value into parts.
             * @param content The text to be further divided and returned as a vector.
             * @param what Splitter.
             */
            static std::vector<std::string> split(const std::string& content, const char what);
    };
}
#endif