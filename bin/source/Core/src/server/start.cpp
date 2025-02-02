/*
#include <crow.h>
#include "Core/server/server_manager.h"
#include "Core/base/utils.h"
#include "Core/base/print.h"
#include "Core/base/users/user_manager.h"
#include "Core/base/command/handler_commands.h"
#include <nlohmann/json.hpp>

#ifdef NRC_JS_WEB

crow::SimpleApp* app;

void serverStart() {
    core::userManager()->userLogin("SpaceOC", "");
    app = new crow::SimpleApp();

    std::string static_dir = ".";

    //app.loglevel(crow::LogLevel::Debug);

    // Роут для статических файлов
    CROW_ROUTE((*app), "/<string>")
    ([static_dir](const crow::request& req, crow::response& res, const std::string& path) {
        std::string file_path = static_dir + "/" + path;
        trace(file_path, DEFAULT_PRINT_COLOR);
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
            res.code = 404;
            res.write("File not found");
            res.end();
            return;
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string::size_type index = file_path.rfind('.');
        if (index!= std::string::npos) {
            std::string extension = file_path.substr(index + 1);
            if (extension == "js") {
                res.set_header("Content-Type", "application/javascript");
            } else if (extension == "css") {
                res.set_header("Content-Type", "text/css");
            } else {
                res.set_header("Content-Type", "text/plain");
            }
        }

        res.write(content);
        res.end();
    });


    CROW_ROUTE((*app), "/system_api/get")
        ([&](const crow::request& req, crow::response& res) {
            auto body = req.body;
            nlohmann::json requestJson = nlohmann::json::parse(body);
            nlohmann::json responseJson;
            if (requestJson.find("call") != requestJson.end()) {
                std::string call = requestJson["call"];
                if (call == "user") {
                    const std::string name = requestJson.at("username").get<std::string>();
                    responseJson["username"] = core::userManager()->getUserMap()[name];
                }
                responseJson["status"] = "success";
                res.set_header("Content-Type", "application/json");
                res.write(responseJson.dump());
                res.end();
            }
            else {
                res.code = 400;
                res.write("Error: Expected 'call' in JSON body"); 
                res.end();
            }
        });

        CROW_ROUTE(app, "/system/create_user/<string>")
        ([&](std::string jsonData) {
            jsonData = core::Utils::replace(jsonData, "%22", "\"");
            jsonData = core::Utils::replace(jsonData, "%7B", "{");
            jsonData = core::Utils::replace(jsonData, "%7D", "}");

            nlohmann::json json = nlohmann::json::parse(jsonData);

            const std::string name = json.at("name").get<std::string>();
            const int permissions = json.at("permissions").get<int>();
            const std::string language = json.at("language").get<std::string>();
            const std::string password = json.at("password").get<std::string>();

            core::userManager()->addUser(name, static_cast<core::Permissions>(permissions));
                        
            trace("user created!", DEFAULT_PRINT_COLOR);
            core::print();
            return crow::response();
        });
    // Роут для обработки POST-запросов
    CROW_ROUTE((*app), "/system_api/post").methods("POST"_method)
    ([](const crow::request& req, crow::response& res){
        auto body = req.body;
        nlohmann::json requestJson = nlohmann::json::parse(body);

        nlohmann::json responseJson;
        if (requestJson.find("call") != requestJson.end()) {
            std::string call = requestJson["call"];
            // Обработка запроса с использованием переданного ключа
            if (call == "create_user") {
                const std::string name = requestJson.at("username").get<std::string>();
                const int permissions = requestJson.at("permissions").get<int>();
                //const std::string language = requestJson.at("language").get<std::string>();
                //const std::string password = requestJson.at("password").get<std::string>();
                
                core::userManager()->addUser(name, static_cast<core::Permissions>(permissions));

                responseJson["status"] = "success";
                if (requestJson.find("get") != requestJson.end() && requestJson.at("get")) {
                    responseJson["username"] = name;
                    responseJson["permissions"] = permissions;
                } 
            }
            else if (call == "delete_user") {
                const std::string name = requestJson.at("username").get<std::string>();
                core::userManager()->deleteUser(name);
            }
            else if (call == "change_user") {
                const std::string name = requestJson.at("username").get<std::string>();
            }
            else if (call == "command") {
                const std::string name = requestJson.at("name").get<std::string>();
                const std::string params = (requestJson.find("params") != requestJson.end() ? requestJson.at("params").get<std::string>() : "");

                std::vector<core::CommandObject> parsedCommand = core::handlerCommands()->parsing(name + (!params.empty() ? " \"" + params + "\"" : ""));
                core::handlerCommands()->sendCommand(parsedCommand.at(0));
            }

            res.set_header("Content-Type", "application/json");
            res.write(responseJson.dump());
            res.end();
        } else {
            responseJson["status"] = "fail";
            responseJson["error-message"] = "Error: Expected 'call' in JSON body";
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.write(responseJson.dump());
            res.end();
        }
    });

    CROW_ROUTE((*app), "/")([static_dir](crow::response& res) {
        std::string html = R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <link rel="stylesheet" href="style.css">
                <script src="Traceable.js"></script>
                <script src="Event.js"></script>
                <script src="Events.js"></script>
                <script src="NRCWindow.js"></script>
                <script src="test.js"></script>
                <title>Your App</title>
            </head>
            <body>
                <h1>Welcome to NRC!</h1>
                <p>JavaScript and CSS are loaded automatically.</p>
            </body>
            </html>
        )";
        res.set_header("Content-Type", "text/html");
        res.write(html);
        res.end();
    });

    (*app).port(18080).multithreaded().run();
}

#endif
*/