#include "Core/base/utils.h"

bool core::Utils::stringIsNumbers(const std::string& content) {
    for (const auto& letter : content) {
        if (!isdigit(letter)) return false;
    }
    return true;
}