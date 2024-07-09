#include <string>
#include "permissions_enum_class.h"
class user {
    private:
        std::string displayName, language, password;
        permissionsEC permissions;
    public:
        bool truePassword(std::string password);
        std::string getDisplayName();
        std::string getLanguage();
        permissionsEC getPermissions();
};