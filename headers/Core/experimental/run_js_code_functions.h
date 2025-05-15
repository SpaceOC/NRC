// is not finished!
#ifndef NRC_EXPERIMENTAL_RUN_JS_CODE_FUNCTIONS_H_
#define NRC_EXPERIMENTAL_RUN_JS_CODE_FUNCTIONS_H_

#include <string>

namespace mjs {
	class interpreter;
	class gc_heap;
};

namespace core {
	class User;
};

namespace core_experimental {
	void addUserManagerFunctions(mjs::interpreter& i, mjs::gc_heap& gc);
	void addPseudoFileSystemFunctions(mjs::interpreter& i, core::User* user, mjs::gc_heap& gc);
	//void addSimpleFileSystemFunctions(mjs::interpreter& i, core::User* user, mjs::gc_heap& gc);
	void addOtherFunctions(mjs::interpreter& i, core::User* user, mjs::gc_heap& gc, std::string& str);
}

#endif