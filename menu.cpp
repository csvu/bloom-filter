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
            std::cout << "-> Invalid Username" << std::endl;
            continuePrompt(loop);
            continue;
        }

        if (!isValidPassword(account.username, account.password)) {
            std::cout << "-> Invalid Password" << std::endl;
            continuePrompt(loop);
            continue;
        }

        if (isPossiblyMember(username_check, account.username)) {
            std::cout << "-> Username already registered" << std::endl;
            continuePrompt(loop);
            continue;
        }

        if (isPossiblyMember(weak_password_check, account.password)) {
            std::cout << "-> Weak Password" << std::endl;
            continuePrompt(loop);
            continue;
        }
        valid = true;
        loop = true;
    }
    if (valid) saveAccountToDatabase(username_check, account.username, account.password);
}

void signUpAccount(const std::string& username, const std::string& password,
                   BloomFilter* username_check, BloomFilter* weak_password_check,
                   std::ofstream& failFile) {
    if (!isValidUsername(username)) {
        std::cout << "-> Invalid Username" << std::endl;
        failFile << "[" << username << "]"
                 << "-"
                 << "[" << password << "]"
                 << " - Invalid Username" << std::endl;
        return;
    }

    if (!isValidPassword(username, password)) {
        std::cout << "-> Invalid Password" << std::endl;
        failFile << "[" << username << "]"
                 << "-"
                 << "[" << password << "]"
                 << " - Invalid Password" << std::endl;
        return;
    }

    if (isPossiblyMember(username_check, username)) {
        std::cout << "-> Username already registered" << std::endl;
        failFile << "[" << username << "]"
                 << "-"
                 << "[" << password << "]"
                 << " - Username already registered" << std::endl;
        return;
    }

    if (isPossiblyMember(weak_password_check, password)) {
        std::cout << "-> Weak Password" << std::endl;
        failFile << "[" << username << "]"
                 << "-"
                 << "[" << password << "]"
                 << "- Weak Password" << std::endl;
        return;
    }

    saveAccountToDatabase(username_check, username, password);
}

void saveAccountToDatabase(BloomFilter* username_check, const std::string& username,
                           const std::string& password) {
    insertMember(username_check, username);
    std::ofstream outFile("data.txt", std::ios_base::app);
    outFile << std::left << std::setw(9) << std::setfill(' ') << username << " " << std::setw(19)
            << std::setfill(' ') << password << std::endl;
    outFile.close();
}

void signUpMultipleAccounts(BloomFilter* username_check, BloomFilter* weak_password_check) {
    int numAccounts;
    std::cout << "Enter the number of accounts you want to sign up: ";
    std::cin >> numAccounts;
    std::cin.ignore();
    std::ofstream failFile("Fail.txt");
    for (int i = 0; i < numAccounts; i++) {
        std::string username, password;
        std::cout << "\nAccount #" << (i + 1) << std::endl;
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        std::cout << "Enter password: ";
        std::getline(std::cin, password);

        signUpAccount(username, password, username_check, weak_password_check, failFile);
    }
    failFile.close();
}

void continuePrompt(bool& loop_out) {
    while (true) {
        std::cout << "Do you want to continue? (y/n): ";
        char choice = '\0';
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 'n') {
            loop_out = true;
            break;
        } else if (choice != 'y') {
            std::cout << "Invalid input!" << std::endl;
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
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        // Sure that username is not a member
        if (!isPossiblyMember(username_check, username)) {
            std::cout << "Wrong username or password!" << std::endl;
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
            std::cout << "Wrong username or password!" << std::endl;
            continuePrompt(loop_out);
            continue;
        }

        if (temp.password == password) {
            std::cout << "Login successfully!" << std::endl;
            is_logged_in = true;
            account = temp;
            break;
        } else {
            std::cout << "Wrong username or password!" << std::endl;
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

        // Disk storage access and replace the old password with the new one
        std::fstream fst("data.txt");
        Account account;

        while (fst >> account.username) {
            if (account.username == username) {
                fst.seekp(-10, std::ios::cur);
                fst << std::left << std::setw(9) << std::setfill(' ') << username << " "
                    << std::setw(19) << std::setfill(' ') << new_password << std::endl;
                break;
            } else {
                fst >> account.password;
            }
        }
        fst.close();

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
        int choice = 0;
        std::cin >> choice;
        std::cin.ignore();

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
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                break;
        }
    }
    destroyBloomFilter(username_check);
    destroyBloomFilter(weak_password_check);
}
