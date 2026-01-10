#include <iostream>
#include "Core/main.h"
#include "Core/print/print.h"

extern void OSStart();

core::Main* nrc;

int main() {
	try
	{
		nrc = new core::Main(OSStart);
		nrc->init();
		nrc->start();
		delete nrc;
	}
	catch(const std::exception& e)
	{
		core::print("ERROR: ", core::PrintColors::red);
		core::print(e.what(), core::PrintColors::red);
	}
	return 0;
}