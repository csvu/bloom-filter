#include "account.h"

bool isValidUsername(const std::string& password, const std::string& username,
                     const std::vector<std::string>& weakPasswords) {
    if (username.length() <= 5 || username.length() >= 10) {
        return false;
    }

    if (username.find(' ') != std::string::npos) {
        return false;
    }

    if (std::find(weakPasswords.begin(), weakPasswords.end(), password) != weakPasswords.end()) {
        return false;
    }

    return true;
}
bool isValidPassword(const std::string& password, const std::string& username,
                     const std::vector<std::string>& weakPasswords) {
    if (password.length() <= 10 || password.length() >= 20) {
        return false;
    }

    if (password.find(' ') != std::string::npos) {
        return false;
    }

    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;
    for (char c : password) {
        if (std::isupper(c)) {
            hasUppercase = true;
        } else if (std::islower(c)) {
            hasLowercase = true;
        } else if (std::isdigit(c)) {
            hasDigit = true;
        } else if (std::ispunct(c) || std::isspace(c)) {
            hasSpecialChar = true;
        }
    }
    if (!(hasUppercase && hasLowercase && hasDigit && hasSpecialChar)) {
        return false;
    }

    if (std::find(weakPasswords.begin(), weakPasswords.end(), password) != weakPasswords.end()) {
        return false;
    }

    if (password == username) {
        return false;
    }

    return true;
}

bool isValidAccount(const std::string& username, const std::string& password,
                    const std::vector<std::string>& existingUsernames,
                    const std::vector<std::string>& weakPasswords) {
    if (!isValidUsername(password, username, weakPasswords)) {
        return false;
    }

    if (!isValidPassword(password, username, weakPasswords)) {
        return false;
    }

    return true;
}