#include "menu.h"
bool isValidUsername(const std::string& password, const std::string& username,
                     const std::vector<std::string>& weakPasswords) {
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
bool isValidPassword(const std::string& password, const std::string& username,
                     const std::vector<std::string>& weakPasswords) {
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

bool isValidAccount(const std::string& username, const std::string& password,
                    const std::vector<std::string>& existingUsernames,
                    const std::vector<std::string>& weakPasswords) {
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

void primeMenu() {
    BloomFilter* username_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);
    BloomFilter* weak_password_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);

    inputByFile(username_check, "SignUp.txt");
    inputByFile(weak_password_check, "Weak-Pass.txt");

    bool is_logged_in = false;
    Account account;

    std::cout << "Welcome to the account manager" << std::endl;

    bool loop_out = false;
    while (!loop_out) {
        std::cout << "Please choose an option" << std::endl;
        std::cout << "1. Sign up" << std::endl;
        std::cout << "2. Log in" << std::endl;
        std::cout << "3. Change password" << std::endl;
        std::cout << "4. Exit" << std::endl;

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                logIn(account, is_logged_in, username_check);
                break;
            case 3:
                if (!is_logged_in) {
                    std::cout << "You must log in first" << std::endl;
                    break;
                }
                changePassword(account.username, weak_password_check);
                break;
            case 4:
                loop_out = true;
                break;
            default:
                std::cout << "Invalid input" << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                break;
        }
    }
}

void signUp() {}

void continuePrompt(bool& loop_out) {
    while (true) {
        std::cout << "Do you want to continue? (y/n): ";
        char choice = '\0';
        std::cin >> choice;
        if (choice == 'n') {
            loop_out = true;
            break;
        } else if (choice != 'y') {
            std::cout << "Invalid input" << std::endl;
            continue;
        } else
            break;
    }
}

void logIn(Account& account, bool& is_logged_in, BloomFilter* username_check) {
    bool loop_out = false;
    while (!loop_out) {
        std::string username, password;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        // Sure that username is not a member
        if (!isPossiblyMember(username_check, username)) {
            std::cout << "Wrong username or password" << std::endl;
            continuePrompt(loop_out);
            continue;
        }

        // Not sure that username is a member, continue to check
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
            continuePrompt(loop_out);
            continue;
        }

        if (account.password == password) {
            std::cout << "Login successful" << std::endl;
            is_logged_in = true;
            break;
        } else {
            std::cout << "Wrong username or password" << std::endl;
            continuePrompt(loop_out);
        }
    }
}

void changePassword(const std::string& username, BloomFilter* weak_password_check) {
    bool loop_out = false;
    while (!loop_out) {
        std::string new_password;
        std::cout << "New password: ";
        std::cin >> new_password;

        if (isPossiblyMember(weak_password_check, new_password)) {
            std::cout << "Your password might be insecure. Please type another password"
                      << std::endl;
            continuePrompt(loop_out);
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
        std::cout << "Password changed successfully" << std::endl;
        loop_out = true;
    }
}
