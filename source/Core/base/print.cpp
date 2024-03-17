#pragma once

#include <iostream>

class PrintManager {
    public:
        virtual void printMessage(const std::string Message) const {
            std::cout << Message << std::endl;
        }
        virtual void printInputMessage(const std::string Message) const {
            std::cout << Message;
        }
        //virtual void convert() const {}
        //virtual void colors() const {}
};