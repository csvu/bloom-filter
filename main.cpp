#include "menu.h"

int main() {
    primeMenu();

    char signUpMultipleOption;
    std::cout << "Do you want to sign up multiple accounts? (y/n): ";
    std::cin >> signUpMultipleOption;

    if (signUpMultipleOption == 'y' || signUpMultipleOption == 'Y') {
        signUpMultipleAccounts(username_check);
    }

    return 0;
}
