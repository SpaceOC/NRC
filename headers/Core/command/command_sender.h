#ifndef NRC_BASE_COMMAND_COMMAND_SENDER_H_
#define NRC_BASE_COMMAND_COMMAND_SENDER_H_

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

namespace core {

class CommandSenderBasic {
	protected:
		int lastError;
		std::string lastMessageError;
	public:
		virtual void zone() const;
};

};
#endif