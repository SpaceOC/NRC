/*
#include <crow.h>
#include <nlohmann/json.hpp>

#ifdef NRC_JS_WEB

void serverStart();

class ServerManager {
    public:
        void start();
        void send(nlohmann::json req);
        nlohmann::json get(std::string what);
};

extern crow::SimpleApp* app;

#endif
*/