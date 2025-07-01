#include <string>
#include <vector>
#include <map>
#include "other/versionc.h"

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

	/**
	 * 0 - Минимальная версия ядра для работы модуля
	 * 1 - Максимальная версия ядра для работы модуля
	 */
	VersionC* uses[2];

	std::map<std::string, std::string> layout;
	std::vector<std::string> translations;
};

};