#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "account.h"
#include "bloom_filter.h"

void continuePrompt();
void logIn(Account& account, BloomFilter* username_check);
void changePassword(const std::string& username, BloomFilter* weak_password_check);