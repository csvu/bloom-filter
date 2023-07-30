#pragma once
#include <string>
#include <vector>

struct Account {
    std::string username;
    std::string password;
};

bool isValidUsername(const std::string& username);
bool isValidPassword(const std::string& username, const std::string& password);
