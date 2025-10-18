#ifndef NRC_MODULES_MODULE_METADATA_H_
#define NRC_MODULES_MODULE_METADATA_H_

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include "Core/other/versionc.h"

namespace core {

/**
 * Хранит все метаданные модуля:
 * Имя, описания, поддерживаемые платформы, мин.-максимальные версии ядра для работы,
 * местонахождение тех или иных файлов для запуска модуля, переводы
 */
struct ModuleMetadata {
	std::string name;
	std::string description;
	std::vector<std::string> targets;
	std::vector<std::string> requiresClasses;

	/**
	 * 0 - Минимальная версия ядра для работы модуля
	 * 1 - Максимальная версия ядра для работы модуля
	 */
	VersionC* uses[2];
	bool requiredVersionInRange = false; // Если стоит true, то требует чтобы версия ядра была не выше и не ниже требуемых модулю. В противном случае модуль не загрузится.

	std::map<std::string, std::string> layout;
	std::vector<std::string> translations;

	void makeMetadataFromJSON(const nlohmann::json& j);
};



}; // namespace core

#endif // NRC_MODULES_MODULE_METADATA_H_