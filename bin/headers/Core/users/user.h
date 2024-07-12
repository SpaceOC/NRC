#include <string>
#include "permissions_enum_class.h"

class user {
    private:
        std::string displayName, language, password;
        permissionsEC permissions;
        bool userIsInit = false;
    public:
        user();
        user(std::string displayName, std::string language, std::string password, permissionsEC permissions);
        bool truePassword(std::string password);
        //bool trueKey(std::string longUserKey);
        std::string getDisplayName();
        std::string getLanguage();
        permissionsEC getPermissions();
};