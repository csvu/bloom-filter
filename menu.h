#pragma once
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "account.h"
#include "bloom_filter.h"

void primeMenu();

void signUpOneAccount(BloomFilter* username_check, BloomFilter* weak_password_check);
void signUpAccount(const std::string& username, const std::string& password,
                   BloomFilter* username_check, BloomFilter* weak_password_check,
                   std::ofstream& failFile);
void signUpMultipleAccounts(BloomFilter* username_check, BloomFilter* weak_password_check);
void logIn(Account& account, bool& is_logged_in, BloomFilter* username_check);
void changePassword(const std::string& username, BloomFilter* weak_password_check);

void continuePrompt(bool& loop_out);
int countSpaces(const std::string& text);
void saveAccountToDatabase(BloomFilter* username_check, const std::string& username,
                           const std::string& password);
