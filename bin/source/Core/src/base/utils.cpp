#include "Core/base/utils.h"

bool core::Utils::existsInVector(const std::vector<std::string>& vector, const std::string& var) {
    for (const std::string& stringVar : vector) {
        if (stringVar == var) return true;
    }
    return false;
}

template<typename T>
bool core::Utils::existsInVector(const std::vector<T>& vector, const T& var) {
    for (const auto& stringVar : vector) {
        if (stringVar == var) return true;
    }
    return false;
}

bool core::Utils::stringIsNumbers(const std::string& content) {
    for (const auto& letter : content) {
        if (!isdigit(letter)) return false;
    }
    return true;
}