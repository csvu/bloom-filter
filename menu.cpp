#include "menu.h"

void signUpOneAccount(BloomFilter* username_check, BloomFilter* weak_password_check) {
    Account account;

    bool loop = false;
    bool valid = false;

    while (!loop) {
        std::cout << "Username : ";
        std::getline(std::cin, account.username);
        std::cout << "Password : ";
        std::getline(std::cin, account.password);

        if (!isValidUsername(account.username)) {
            std::cout << "Your username is invalid. Please type another username" << std::endl;
            continuePrompt(loop);
            continue;
        }

        if (!isValidPassword(account.username, account.password)) {
            std::cout << "Your password is invalid. Please type another password" << std::endl;
            continuePrompt(loop);
            continue;
        }

        if (isPossiblyMember(username_check, account.username)) {
            std::cout << "Username already registered. Please type another username" << std::endl;
            continuePrompt(loop);
            continue;
        }

        if (isPossiblyMember(weak_password_check, account.password)) {
            std::cout << "Your password might be insecure. Please type another password"
                      << std::endl;
            continuePrompt(loop);
            continue;
        }
        valid = true;
        loop = true;
    }
    if (valid) {
        std::cout << "Sign up successfully!" << std::endl;
        saveAccountToDatabase(username_check, account.username, account.password);
    }
}

void signUpAccount(const std::string& username, const std::string& password,
                   BloomFilter* username_check, BloomFilter* weak_password_check,
                   std::ofstream& failFile) {
    if (!isValidUsername(username)) {
        failFile << username << " " << password << " - Invalid Username" << std::endl;
        return;
    }

    if (!isValidPassword(username, password)) {
        failFile << username << " " << password << " - Invalid Password" << std::endl;
        return;
    }

    if (isPossiblyMember(username_check, username)) {
        failFile << username << " " << password << " - Username already registered" << std::endl;
        return;
    }

    if (isPossiblyMember(weak_password_check, password)) {
        failFile << username << " " << password << "- Weak Password" << std::endl;
        return;
    }

    saveAccountToDatabase(username_check, username, password);
}

void saveAccountToDatabase(BloomFilter* username_check, const std::string& username,
                           const std::string& password) {
    insertMember(username_check, username);
    std::ofstream outFile("data.txt", std::ios_base::app);
    outFile << username << " " << password << std::endl;
    outFile.close();
}

int countSpaces(const std::string& text) {
    int spaceCount = 0;
    for (char c : text) {
        if (c == ' ') {
            spaceCount++;
        }
    }
    return spaceCount;
}
void signUpMultipleAccounts(BloomFilter* username_check, BloomFilter* weak_password_check) {
    std::ofstream failFile("Fail.txt");
    std::ifstream signupFile("SignUp.txt");
    if (!signupFile.is_open()) {
        std::cout << "Error opening SignUp.txt file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(signupFile, line)) {
        std::istringstream iss(line);
        std::string username, password;
        if (countSpaces(line) > 1) {
            failFile << line << " - Wrong username or password!";
        } else {
            iss >> username >> password;
            signUpAccount(username, password, username_check, weak_password_check, failFile);
        }
    }
    std::cout << "Sign up multiple accounts successfully!" << std::endl;
    failFile.close();
    signupFile.close();
}

void continuePrompt(bool& loop_out) {
    while (true) {
        std::cout << "Do you want to continue? (y/n): ";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice.size() != 1 || (choice[0] != 'y' && choice[0] != 'n')) {
            std::cout << "Invalid input!" << std::endl;
            continue;
        } else if (choice[0] == 'n') {
            loop_out = true;
            break;
        } else {
            break;
        }
    }
}

void logIn(Account& account, bool& is_logged_in, BloomFilter* username_check) {
    bool loop_out = false;
    while (!loop_out) {
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        // Sure that username is not a member
        if (!isPossiblyMember(username_check, username)) {
            std::cout << "Wrong username!" << std::endl;
            continuePrompt(loop_out);
            continue;
        }

        // Not sure that username is a member, continue to check
        std::ifstream inp("data.txt");
        bool is_member = false;
        Account temp;
        while (inp >> temp.username >> temp.password) {
            if (temp.username == username) {
                is_member = true;
                break;
            }
        }
        inp.close();

        if (!is_member) {
            std::cout << "Wrong username!" << std::endl;
            continuePrompt(loop_out);
            continue;
        }

        if (temp.password == password) {
            std::cout << "Login successfully!" << std::endl;
            is_logged_in = true;
            account = temp;
            break;
        } else {
            std::cout << "Wrong password!" << std::endl;
            continuePrompt(loop_out);
        }
    }
}

void changePassword(const std::string& username, BloomFilter* weak_password_check) {
    bool loop_out = false;
    while (!loop_out) {
        std::string new_password;
        std::cout << "New password: ";
        std::getline(std::cin, new_password);

        if (!isValidPassword(username, new_password)) {
            std::cout << "Your password is invalid. Please type another password" << std::endl;
            continuePrompt(loop_out);
            continue;
        }

        if (isPossiblyMember(weak_password_check, new_password)) {
            std::cout << "Your password might be insecure. Please type another password"
                      << std::endl;
            continuePrompt(loop_out);
            continue;
        }

        // Disk storage access
        std::ifstream inp("data.txt");
        std::vector<Account> accounts;
        Account account;

        while (inp >> account.username >> account.password) {
            accounts.push_back(account);
        }
        inp.close();

        // Replace the old password with the new one
        std::ofstream out("data.txt");
        for (const auto& acc : accounts) {
            if (acc.username == username) {
                out << acc.username << " " << new_password << "\n";
            } else {
                out << acc.username << " " << acc.password << "\n";
            }
        }
        out.close();

        std::cout << "Password changed successfully" << std::endl;
        loop_out = true;
    }
}

void primeMenu() {
    BloomFilter* username_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);
    BloomFilter* weak_password_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);

    inputByFileUsername(username_check, "data.txt");
    inputByFileWeakPassword(weak_password_check, "Weak-Pass.txt");

    bool is_logged_in = false;
    Account account;

    std::cout << "Welcome to the account manager" << std::endl;

    bool loop_out = false;
    while (!loop_out) {
        std::cout << "========================================" << std::endl;
        if (!is_logged_in) {
            std::cout << "<You are not logged in>" << std::endl;
        } else {
            std::cout << "<You are logged in as " << account.username << ">" << std::endl;
        }
        std::cout << "Please choose an option" << std::endl;
        std::cout << "1. Sign up one account" << std::endl;
        std::cout << "2. Sign up multiple accounts" << std::endl;
        std::cout << "3. Log in" << std::endl;
        std::cout << "4. Change password" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Enter your choice: ";
        std::string str_choice;
        std::getline(std::cin, str_choice);
        int choice = 0;
        if (str_choice.size() != 1 || !std::isdigit(str_choice[0])) {
            choice = -1;
        } else {
            choice = std::stoi(str_choice);
        }

        switch (choice) {
            case 1:
                signUpOneAccount(username_check, weak_password_check);
                break;
            case 2:
                signUpMultipleAccounts(username_check, weak_password_check);
                break;
            case 3:
                logIn(account, is_logged_in, username_check);
                break;
            case 4:
                if (!is_logged_in) {
                    std::cout << "You must log in first!" << std::endl;
                    break;
                }
                changePassword(account.username, weak_password_check);
                break;
            case 5:
                loop_out = true;
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }
    }
    destroyBloomFilter(username_check);
    destroyBloomFilter(weak_password_check);
}
