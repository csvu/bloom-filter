#include "menu.h"

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
