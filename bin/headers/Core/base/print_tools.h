#ifndef NRC_BASE_PRINT_TOOLS_H_
#define NRC_BASE_PRINT_TOOLS_H_
#include <string>
#include <filesystem>
#include "Core/base/printable.h"
#include "Core/base/utils.h"

#define FILENAME                    (std::filesystem::path(__FILE__).filename().string())
#define DEFAULT_PRINT_COLOR         core::PrintColors::white

template<typename T>
using is_not_printable_class = std::enable_if_t<!std::is_base_of_v<core::PrintableClass, T>, bool>;

namespace core {
    // colors for the print class
    enum PrintColors {
        black, white, blue, yellow,
        red, aqua, purple, green, grey,
        light_blue, light_green, light_aqua, light_red,
        light_purple, light_yellow, bright_white 
    };

    class ColorsSequence {
        public:
            static const std::string black, red, green, yellow, blue;
            static const std::string purple, aqua, white, grey;
            static const std::string light_red, light_green, light_yellow, light_blue;
            static const std::string light_purple, light_aqua,bright_white;
    };

    std::string getColorEscapeSequence(const PrintColors& color);
    std::string removeEscapeSequence(const std::string& text);
}

#endif