#include "menu.h"

int main() {
    BloomFilter* username_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);
    BloomFilter* weak_password_check = createBloomFilter(FILTER_SIZE, HASH_COUNT);

    inputByFile(username_check, "SignUp.txt");
    inputByFile(weak_password_check, "Weak-Pass.txt");

    Account account;
    logIn(account, username_check);

    continuePrompt();

    changePassword(account.username, weak_password_check);

    return 0;
}
