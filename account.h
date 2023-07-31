#pragma once
#include <string>
#include <vector>

struct Account {
    char username[9]{};
    char password[19]{};
};

bool isValidUsername(const std::string& username);
bool isValidPassword(const std::string& username, const std::string& password);
