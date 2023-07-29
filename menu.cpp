#include "menu.h"
#include <fstream>
#include <sstream>


bool signUpAccount(const std::string& username, const std::string& password, const std::vector<std::string>& existingUsernames, const std::vector<std::string>& weakPasswords, std::ofstream& failFile) {
    if (!isValidUsername(password, username, weakPasswords)) {
        failFile << username << " " << password << " - Invalid Username" << std::endl;
        return false;
    }

    if (!isValidPassword(password, username, weakPasswords)) {
        failFile << username << " " << password << " - Invalid Password" << std::endl;
        return false;
    }

    if (isUsernameRegistered(username, existingUsernames)) {
        failFile << username << " " << password << " - Username already registered" << std::endl;
        return false;
    }

    saveAccountToDatabase(username, password);
    return true;
}

void readExistingUsernames(std::vector<std::string>& existingUsernames) {
    std::ifstream file("SignUp.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file SignUp.txt" << std::endl;
        return;
    }

    existingUsernames.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username;
        iss >> username;
        existingUsernames.push_back(username);
    }

    file.close();
}

void saveAccountToDatabase(const std::string& username, const std::string& password) {
    std::ofstream outFile("Sign Up.txt", std::ios_base::app);
    outFile << username << " " << password << std::endl;
    outFile.close();
}

void readWeakPasswords(std::vector<std::string>& weakPasswords) {
    std::ifstream file("Weak-Pass.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file Weak-Pass.txt" << std::endl;
        return;
    }

    weakPasswords.clear(); 

    std::string password;
    while (file >> password) {
        weakPasswords.push_back(password);
    }

    file.close();
}


void signUpMultipleAccounts(BloomFilter* username_check) {
    int numAccounts;
    std::cout << "Enter the number of accounts you want to sign up: ";
    std::cin >> numAccounts;

    std::vector<std::string> existingUsernames;
    readExistingUsernames(existingUsernames);

    std::vector<std::string> weakPasswords;
    readWeakPasswords(weakPasswords);

    std::ofstream failFile("Fail.txt");
    for (int i = 0; i < numAccounts; i++) {
        std::string username, password;
        std::cout << "\nAccount #" << (i + 1) << std::endl;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (isPossiblyMember(username_check, username)) {
            failFile << username << " " << password << " - Username possibly exists" << std::endl;
        } else {
            insertMember(username_check, username);
            if (signUpAccount(username, password, existingUsernames, weakPasswords, failFile)) {
                std::cout << "Account registered successfully!" << std::endl;
            }
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

    std::cout << "Welcome to the password manager" << std::endl;

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
                signUpMultipleAccounts(username_check);
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
    destroyBloomFilter(username_check);
    destroyBloomFilter(weak_password_check);
}

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
        break;
    }
}
