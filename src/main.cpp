#include <vector>
#include <iostream>

#include "bank_service.h"
#include "ui.h"
#include "auth.h"
#include "persistence.h"

int main() {
    using namespace bank;

    Bank bank;
    initBank(bank);

    // 1) Try to load existing data (accounts + histories)
    loadBankFromFiles(bank, "accounts.csv", "transactions.csv");

    // 2) Initialize list of system users and perform login
    std::vector<User> users;
    initUsers(users);

    // Important: Before using std::getline in login(), make sure
    // there is no leftover newline in cin (on some setups).
    // Here it's safe because we haven't read anything yet.

    if (!login(users)) {
        destroyBank(bank);
        return 0;
    }

    // 3) After successful login, run the main banking menu
    runMainMenu(bank);

    // 4) Save current state before exit
    if (!saveBankToFiles(bank, "accounts.csv", "transactions.csv")) {
        std::cerr << "Warning: failed to save bank data.\n";
    }

    destroyBank(bank);
    return 0;
}
