#ifndef NRC_BASE_MAIN_H_
#define NRC_BASE_MAIN_H_

namespace core {
    class main {
        private:
            static void fixNOW();
            
            // Adding all standard commands.
            static void addCommands();
            static void addCRules();
        public:
            main();
            ~main();
    };
}

#endif