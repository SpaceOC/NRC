#ifndef NRC_BASE_COMMAND_COMMAND_SENDER_H_
#define NRC_BASE_COMMAND_COMMAND_SENDER_H_

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

namespace core {
	class CommandSenderBasic {
	private:
		mutable std::string _lastMessageError;
		//int _lastError;
	public:
		virtual void zone() const;
		std::string lastMessageError() { return _lastMessageError; }
	};
};
#endif