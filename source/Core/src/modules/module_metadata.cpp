#include "Core/modules/module_metadata.h"

void core::ModuleMetadata::makeMetadataFromJSON(const nlohmann::json& j) {
    if (!j.count("name") || !j.count("description") || !j.count("targets") || !j.count("uses") || !j.count("translations")) {
        return;
    }
    name = j["name"].get<std::string>();
    description = j["description"].get<std::string>();

    for (nlohmann::json target : j["targets"]) {
        targets.push_back(target.get<std::string>());
    }
    for (nlohmann::json lang : j["translations"]) {
        targets.push_back(lang.get<std::string>());
    }

    std::vector<std::string> usesValues;
    for (nlohmann::json use : j["uses"]) {
        usesValues.push_back(use.get<std::string>());
    }

    for (size_t i = 0; i < 2; i++) {
        if (usesValues.size() == 2)
            uses[i] = new VersionC(usesValues[i]);
        else
            uses[i] = new VersionC(usesValues[0]);
    }

    requiredVersionInRange = j.count("required_version_in_range") ? j["required_version_in_range"].get<bool>() : false;

    if (j.count("layout")) {
        if (j["layout"].count("windows")) layout["windows"] = j["layout"]["windows"].get<std::string>();
        if (j["layout"].count("linux")) layout["linux"] = j["layout"]["linux"].get<std::string>();
        if (j["layout"].count("macos")) layout["macos"] = j["layout"]["macos"].get<std::string>();
    }

    if (j.count("requires_classes")) {
        for (nlohmann::json requiredClass : j["requires_classes"]) {
            requiresClasses.push_back(requiredClass.get<std::string>());
        }
    }
}