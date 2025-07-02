#ifndef NRC_BASE_LANGUAGE_UTILS_H_
#define NRC_BASE_LANGUAGE_UTILS_H_

#include <string>

namespace core {

namespace language_util {
    std::string getLC(const std::string_view& c);
    std::string getNameFromLC(const std::string_view& c);
};

}

#endif // NRC_BASE_LANGUAGE_UTILS_H_