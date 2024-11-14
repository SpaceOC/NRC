#ifndef NRC_BASE_PRINTABLE_H_
#define NRC_BASE_PRINTABLE_H_
#include <ostream>

namespace core {

class PrintableClass {
    public:
        virtual ~PrintableClass() {}
        friend std::ostream& operator<<(std::ostream& os, PrintableClass& yourClass) {
            os << yourClass.toString();
            return os;
        }
    private:
        virtual std::string toString() const { return "[PrintableClass]"; }
};

}

#endif