#include <iostream>
#include <nlohmann/json.hpp>
#include "Core/utils/other_util.h"
#include "Core/utils/language_util.h"

static nlohmann::json* localeCodeJson;

std::string core::language_util::getLC(const std::string_view& c) {
    std::string r = other_util::getFileContent("data/locales.json");
    if (r.empty()) {
        return "";
    }

    if (!localeCodeJson) {
        localeCodeJson = new nlohmann::json(nlohmann::json::parse(r));
    }

    for (const auto& a : *localeCodeJson) {
        if (a["language"]["name"].get<std::string>() != c) {
            continue;
        }

        return a["locale"];
    }

    return "";
}

std::string core::language_util::getNameFromLC(const std::string_view& c) {
    std::string r = other_util::getFileContent("data/locales.json");
    if (r.empty()) {
        return "";
    }

    if (!localeCodeJson) {
        localeCodeJson = new nlohmann::json(nlohmann::json::parse(r));
    }

    for (const auto& a : *localeCodeJson) {
        if (a["locale"].get<std::string>() != c) {
            continue;
        }

        return a["language"]["name"];
    }

    return "";
}