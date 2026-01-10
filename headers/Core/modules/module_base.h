#ifndef NRC_MODULES_MODULE_BASE_H_
#define NRC_MODULES_MODULE_BASE_H_
// TODO: Перевести все комментарии на английский язык... А хотя... зачем?
#include <string>
#include <memory>
#include <map>

namespace core {
	class Main;
	struct ModuleMetadata;

class ModuleBase {
	public:
		explicit ModuleBase(ModuleMetadata* meta, Main* core);
		//explicit ModuleBase() {} // временно используется для тестов
		virtual ~ModuleBase() {};
		ModuleBase(const ModuleBase &) = delete;
		ModuleBase(ModuleBase &&) = delete;
		ModuleBase &operator=(const ModuleBase &) = delete;
		ModuleBase &operator=(ModuleBase &&) = delete;

		/**	Принимает и выполняет определённый запрос от системы
		 * @param request Запрос
		 */
		virtual bool runRequest(const std::string&) { return false; }

		/**	Обработка загрузки модуля из ядра
		 */
		virtual void onLoad() {}

		/**	Обработка выгрузки модуля из ядра
		 */
		virtual void onUnload() {};

		/**	Обработка неправильного запроса к ядру
		 */
		virtual void onWrongRequest(int) {};

		/**	Запрашивание доступного указателя с необходимым классом.
		 * @param request Название запрашиваемого класса
		 * @return Возвращает указатель...?
		 */
		void* requestPtrClass(const std::string& request);

		ModuleMetadata getMetadata();
	private:
		std::string _path;
		std::map<std::string, std::string> _currentLanguage;
		ModuleMetadata* _metadata;
		Main* _core;
};

}; // namespace core

#endif // NRC_MODULES_MODULE_BASE_H_