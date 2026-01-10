#ifndef NRC_OTHER_VERSIONC_H_
#define NRC_OTHER_VERSIONC_H_

#include <string>

namespace core {

/**
 * Вспомогательный класс используемый (на данный момент)
 * только для проверки версий ядра (на которых сам модуль может запускаться) различных модулей
 */
class VersionC {
	public:
		VersionC() = delete;

		/**
		 * @param r версия в виде типа std::string (примеры: 1.0; 0.1; 3.5.1)
		 */
		VersionC(const std::string& r) : _versionStr(r) {
			_calculatedVer = convVersionStrToDouble(r);
		}

		constexpr bool operator==(const VersionC& ver) {
			return (this->_versionStr == ver._versionStr);
		}

		constexpr bool operator!=(const VersionC& ver) {
			return (this->_versionStr != ver._versionStr);
		}

		constexpr bool operator<(const VersionC& ver) {
			double thisV = convVersionStrToDouble(_versionStr);
			double v = convVersionStrToDouble(ver._versionStr);

			return (thisV < v);
		}

		constexpr bool operator>(const VersionC& ver) {
			double thisV = convVersionStrToDouble(_versionStr);
			double v = convVersionStrToDouble(ver._versionStr);

			return (thisV > v);
		}

		constexpr bool operator<=(const VersionC& ver) {
			double thisV = convVersionStrToDouble(_versionStr);
			double v = convVersionStrToDouble(ver._versionStr);

			return (thisV <= v);
		}

		constexpr bool operator>=(const VersionC& ver) {
			double thisV = convVersionStrToDouble(_versionStr);
			double v = convVersionStrToDouble(ver._versionStr);

			return (thisV >= v);
		}


		std::string getVersionStr() { return _versionStr; }
		operator std::string() { return _versionStr; }
		operator double() { return _calculatedVer; }
	private:
		std::string _versionStr;
		double _calculatedVer;

		/**
		 * Преобразует std::string версии в double для последующих операции != == <= >=
		 */
		double convVersionStrToDouble(const std::string& versionStr) {
			if (versionStr.empty()) return 0.0;
			double result = 0.f;

			// rd = 10 * rd (если rd равен 0, то умножается на 1)
			int rd = 0;
			for (size_t i = 0; i < versionStr.size(); i++) {
				if (!isdigit(versionStr[i])) continue;

				// э, а я не знаю как по другому
				std::string sub = versionStr.substr(i, i + 1);
				const char* a = sub.c_str();

				int r = atoi(a);
				if (r != 0) result += r / (rd);
				rd += 10 * (!rd ? 1 : rd);
			}

			return result;
		}
};

}

#endif