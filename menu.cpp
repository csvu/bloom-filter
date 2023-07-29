#include "menu.h"

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
        break;
    }
}
