#include "account.h"

bool isValidUsername(const std::string& username) {
    if (username.length() <= 5 || username.length() >= 10) {
        return false;
    }

    if (username.find(' ') != std::string::npos) {
        return false;
    }

    return true;
}

bool isValidPassword(const std::string& username, const std::string& password) {
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
        } else if (!std::isalpha(c)) {
            hasSpecialChar = true;
        }
    }
    if (!(hasUppercase && hasLowercase && hasDigit && hasSpecialChar)) {
        return false;
    }

    if (password == username) {
        return false;
    }

    return true;
}
