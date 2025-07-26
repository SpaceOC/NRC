#ifndef NRC_MODULES_MODULE_BASE_H_
#define NRC_MODULES_MODULE_BASE_H_
// TODO: Перевести все комментарии на английский язык... А хотя... зачем?
#include <string>
#include <memory>
#include <map>

namespace core {
	struct ModuleMetadata;

class ModuleBase {
	public:
		explicit ModuleBase(ModuleMetadata* meta);
		virtual ~ModuleBase() = 0;
		ModuleBase(const ModuleBase &) = delete;
		ModuleBase(ModuleBase &&) = delete;
		ModuleBase &operator=(const ModuleBase &) = delete;
		ModuleBase &operator=(ModuleBase &&) = delete;

		/**	Принимает и выполняет определённый запрос от системы
		 * @param request Запрос
		 */
		virtual bool runRequest(const std::string &request) = 0;

		/**	Обработка загрузки модуля из ядра
		 */
		virtual void onLoad() = 0;

		/**	Обработка выгрузки модуля из ядра
		 */
		virtual void onUnload() = 0;

		/**	Обработка неправильного запроса к ядру
		 */
		virtual void onWrongRequest() = 0;

		/**	Запрашивание доступного указателя с необходимым классом.
		 * @param request Название запрашиваемого класса
		 * @note Изменяйте эту функцию, только если надо использовать PFS для работы модуля
		 */
		virtual void requestPtrClass(const std::string& request) = 0;
	private:
		std::string path;
		std::map<std::string, std::string> currentLanguage;
		std::unique_ptr<ModuleMetadata> metadata;
};

}; // namespace core

#endif // NRC_MODULES_MODULE_BASE_H_