#include "menu.h"
bool isValidUsername(const std::string& password, const std::string& username, const std::vector<std::string>& weakPasswords) {
 
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
bool isValidPassword(const std::string& password, const std::string& username, const std::vector<std::string>& weakPasswords) {

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

bool isValidAccount(const std::string& username, const std::string& password, const std::vector<std::string>& existingUsernames, const std::vector<std::string>& weakPasswords) {

    if (!isValidUsername(password, username, weakPasswords)) {
        return false;
    }

    if (!isValidPassword(password, username, weakPasswords)) {
        return false;
    }

    return true;
}

void saveAccountToDatabase(const std::string& username, const std::string& password) {
    std::ofstream outFile("Sign Up.txt", std::ios_base::app);
    outFile << username << " " << password << std::endl;
    outFile.close();
}

void signUpMultipleAccounts(BloomFilter* bloom_filter) {
    int numAccounts;
    std::cout << "Enter the number of accounts you want to sign up: ";
    std::cin >> numAccounts;

    std::ofstream failFile("Fail.txt");
    for (int i = 0; i < numAccounts; i++) {
        std::string username, password;
        std::cout << "\nAccount #" << (i + 1) << std::endl;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (isPossiblyMember(bloom_filter, username)) {
            failFile << username << " " << password << std::endl;
        } else {
            insertMember(bloom_filter, username);
            saveAccountToDatabase(username, password);
            std::cout << "Account registered successfully!" << std::endl;
        }
    }
    failFile.close();
}

void continuePrompt() {
    while (true) {
        std::cout << "Do you want to continue? (y/n): ";
        char choice = '\0';
        std::cin >> choice;
        if (choice == 'n')
            exit(0);
        else if (choice != 'y') {
            std::cout << "Invalid input" << std::endl;
            continue;
        } else
            break;
    }
}

void logIn(Account& account, BloomFilter* username_check) {
    while (true) {
        std::string username, password;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        if (!isPossiblyMember(username_check, username)) {
            std::cout << "Wrong username or password" << std::endl;
            continuePrompt();
            continue;
        }

        std::ifstream inp("SignUp.txt");
        bool is_member = false;
        while (inp >> account.username >> account.password) {
            if (account.username == username) {
                is_member = true;
                break;
            }
        }
        inp.close();

        if (!is_member) {
            std::cout << "Wrong username or password" << std::endl;
            continuePrompt();
            continue;
        }

        if (account.password == password) {
            std::cout << "Login successful" << std::endl;
            break;
        } else {
            std::cout << "Wrong username or password" << std::endl;
            continuePrompt();
        }
    }
}

void changePassword(const std::string& username, BloomFilter* weak_password_check) {
    while (true) {
        std::string new_password;
        std::cout << "New password: ";
        std::cin >> new_password;

        if (isPossiblyMember(weak_password_check, new_password)) {
            std::cout << "Weak password" << std::endl;
            continuePrompt();
            continue;
        }

        std::fstream fst("SignUp.txt");
        std::vector<Account> accounts;
        Account account;

        while (fst >> account.username) {
            fst >> account.password;
            accounts.push_back(account);
        }

        fst.clear();
        fst.seekp(0, std::ios::beg);

        for (const auto& acc : accounts) {
            if (acc.username == username) {
                fst << acc.username << " " << new_password << "\n";
            } else {
                fst << acc.username << " " << acc.password << "\n";
            }
        }

        fst.close();
        break;
    }
}
