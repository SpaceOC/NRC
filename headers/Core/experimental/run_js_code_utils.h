// is not finished!
#ifndef NRC_DISABLE_EXPERIMENTAL_FEATURES
#ifndef NRC_EXPERIMENTAL_RUN_JS_CODE_UTILS_H_
#define NRC_EXPERIMENTAL_RUN_JS_CODE_UTILS_H_
#include <string>

namespace mjs {
	class value;
	class string;
	class gc_heap;
};

namespace core {
	class User;
};

namespace core::experimental {
	mjs::value getCString(std::string str, mjs::gc_heap& gc);
	mjs::value objectUserData(mjs::gc_heap& gc, core::User* targetUser);
};
#endif // NRC_EXPERIMENTAL_RUN_JS_CODE_UTILS_H_
#endif // NRC_DISABLE_EXPERIMENTAL_FEATURES