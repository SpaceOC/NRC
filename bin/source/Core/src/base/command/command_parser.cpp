#include "Core/base/print.h"
#include "Core/base/command/command_parser.h"
#include "Core/logs.h"

std::string core::getStringInQ(const std::vector<std::string>& splittedText, int& count) {
    std::string str;
    try {
        bool quoteOpened;
        for (std::string txt : splittedText) {
            if (!txt._Starts_with("\"") && txt.back() != '"' && quoteOpened) {
                str += " " + txt;
                ++count;
                continue;
            }
            if (txt._Starts_with("\"") && !quoteOpened) {
                if (!str.empty()) break;
                quoteOpened = true;
                str += txt.substr(1, txt.back() == '"' ? txt.length() - 2 : txt.length());
                ++count;
                if (txt.back() == '"') quoteOpened = false;
            }
            else if (txt.back() == '"' && quoteOpened) {
                str += " " + txt.substr(0, txt.length() - 1);
                ++count;
                quoteOpened = false;
            }
        }
    }
    catch(const std::exception& e) {
        trace(core::PrintColors::red,
            "Error: ", e.what(), "\n",
            "splittedText: ", splittedText, "\n"
            "str: ", str
        );
		#ifdef NRC_WEB
		outputLog += "getStringInQ Error: " + std::string(e.what()) +
            "\nsplittedText: " + core::Utils::vectorToString(splittedText) + "\nstr: " + str + "\n";
		#endif
    }
    return str;
}

std::pair<std::string, std::string> core::getMapArg(const std::string&) {
    return {};
}