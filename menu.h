#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "account.h"
#include "bloom_filter.h"
void saveAccountToDatabase(const std::string& username, const std::string& password);
bool isValidUsername(const std::string& password, const std::string& username, const std::vector<std::string>& weakPasswords);
bool isValidPassword(const std::string& password, const std::string& username, const std::vector<std::string>& weakPasswords);
bool isValidAccount(const std::string& username, const std::string& password,const std::vector<std::string>& existingUsernames,const std::vector<std::string>& weakPasswords);
void saveAccountToDatabase(const std::string& username, const std::string& password); 
void signUpMultipleAccounts(BloomFilter* bloom_filter);
void continuePrompt();
void logIn(Account& account, BloomFilter* username_check);
void changePassword(const std::string& username, BloomFilter* weak_password_check);