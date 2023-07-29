#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "account.h"
#include "bloom_filter.h"

void primeMenu();
void continuePrompt(bool& loop_out);
void signUp();
void logIn(Account& account, bool& is_logged_in, BloomFilter* username_check);
void changePassword(const std::string& username, BloomFilter* weak_password_check);