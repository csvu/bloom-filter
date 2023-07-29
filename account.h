#pragma once

#include <string>
#include <vector>

struct Account {
    std::string username;
    std::string password;
};

bool isValidUsername(const std::string& password, const std::string& username, const std::vector<std::string>& weakPasswords);
bool isValidPassword(const std::string& password, const std::string& username, const std::vector<std::string>& weakPasswords);
bool isValidAccount(const std::string& username, const std::string& password, const std::vector<std::string>& existingUsernames,  const std::vector<std::string>& weakPasswords);