#include <iostream>
#include "../Core/base/command/HandlerCommands.cpp"

void HelloWorld() {
	std::cout << "Hello World!" << std::endl;
}

void OS() {
	HandlerCommands HC;
	HC.addCommand("hi", "hello world!", HelloWorld);
	std::cout << "Welcome to Your OS" << std::endl;
	std::string temp;
	while (true) {
		std::cout << "Enter command: ";
		std::cin >> temp;
		if (temp == "exit") {
			break;
		}
		else {
			HC.sendCommand(temp);
		}
	}
}