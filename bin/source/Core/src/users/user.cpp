#include "Core/users/user.h"

user::user() {}
user::user(std::string displayName, std::string language, std::string password, permissionsEC permissions) {
    if (!userIsInit) {
        this->displayName = displayName; this->language = (language.size() < 1 ? "English" : language);
        this->password = (password.size() < 1 ? "" : password); this->permissions = permissions;
        userIsInit = true;
    }
}
bool user::truePassword(std::string password) { return password == this->password; }
std::string user::getDisplayName() { return this->displayName; }
std::string user::getLanguage() { return this->language; }
permissionsEC user::getPermissions() { return this->permissions; }