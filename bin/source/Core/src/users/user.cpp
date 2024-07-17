#include "Core/users/user.h"

user::user() {}
user::user(std::string displayName, permissionsEC permissions, std::string language, std::string password) {
    if (!userCreated) {
        this->displayName = displayName; this->permissions = permissions;
        this->language = language; this->password = password;
        userCreated = true;
    }
}
void user::editDisplayName(std::string newDisplayName) { this->displayName = newDisplayName; }
void user::editLanguage(std::string newLanguage) { this->language = newLanguage; }
void user::editPassword(std::string newPassword) { this->password = newPassword; }
void user::editPermissions(permissionsEC newPermissions) { this->permissions = newPermissions; }
bool user::truePassword(std::string password) { return password == this->password; }
bool user::havePassword() { return password.size() > 1 || password != ""; }
std::string user::getDisplayName() { return this->displayName; }
std::string user::getLanguage() { return this->language; }
permissionsEC user::getPermissions() { return this->permissions; }