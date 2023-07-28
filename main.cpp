#include "menu.h"

int main() {
    BloomFilter* username_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);
    BloomFilter* weak_password_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);

    inputByFile(username_check, "SignUp.txt");
    inputByFile(weak_password_check, "Weak-Pass.txt");

  Account account;
    logIn(account, username_check);

    char changePasswordOption;
    std::cout << "Do you want to change your password? (y/n): ";
    std::cin >> changePasswordOption;

    if (changePasswordOption == 'y' || changePasswordOption == 'Y') {
        changePassword(account.username, weak_password_check);
        std::cout << "Password changed successfully!\n";
    }

    continuePrompt(); 

    char signUpMultipleOption;
    std::cout << "Do you want to sign up multiple accounts? (y/n): ";
    std::cin >> signUpMultipleOption;

    if (signUpMultipleOption == 'y' || signUpMultipleOption == 'Y') {
        signUpMultipleAccounts(username_check);
    }

    continuePrompt(); 

    return 0;
}
