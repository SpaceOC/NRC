#ifndef NRC_OTHER_VERSIONC_H_
#define NRC_OTHER_VERSIONC_H_

#include <string>

namespace core {

class VersionC {
	private:
		std::string versionStr;
	public:
		VersionC(const std::string& r) : versionStr(r) {};

		constexpr bool operator==(const VersionC& ver) {
			return (this->versionStr == ver.versionStr);
		}

		constexpr bool operator!=(const VersionC& ver) {
			return (this->versionStr != ver.versionStr);
		}

		constexpr bool operator<(VersionC& ver) {
			double thisV = convVersionStrToDouble();
			double v = ver.convVersionStrToDouble();

			return (thisV < v);
		}

		constexpr bool operator>(VersionC& ver) {
			double thisV = convVersionStrToDouble();
			double v = ver.convVersionStrToDouble();

			return (thisV > v);
		}

		constexpr bool operator<=(VersionC& ver) {
			double thisV = convVersionStrToDouble();
			double v = ver.convVersionStrToDouble();

			return (thisV <= v);
		}

		constexpr bool operator>=(VersionC& ver) {
			double thisV = convVersionStrToDouble();
			double v = ver.convVersionStrToDouble();

			return (thisV >= v);
		}

		const std::string& getVersionStr() { return versionStr; }

		double convVersionStrToDouble() {
			if (versionStr.empty()) return 0.0;
			double result = 0.f;

			int rd = 0;
			for (size_t i = 0; i < versionStr.size(); i++) {
				if (!isdigit(versionStr[i])) continue;

				const char* a = versionStr.substr(i, i + 1).c_str();
				int r = atoi(a);
				if (r != 0) result += r / (rd);
				rd += 10 * (!rd ? 1 : rd);
			}

			return result;
		}
};

}

#endif