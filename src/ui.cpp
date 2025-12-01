#include "ui.h"

#include <iostream>
#include <limits>

#include "persistence.h" // for saveBankToFiles

namespace bank {

namespace {

// Helper: get a valid integer from user with prompt.
int askInt(const std::string& prompt) {
    int value{};
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput(); // clear trailing newline
            return value;
        }
        std::cout << "Invalid integer. Please try again.\n";
        clearInput(); // reset stream and discard bad input
    }
}

// Helper: get a valid double from user with prompt.
double askDouble(const std::string& prompt) {
    double value{};
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        std::cout << "Invalid number. Please try again.\n";
        clearInput();
    }
}

// Helper: read a full line (for names, etc.).
std::string askLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);
    return line;
}

void printMenu() {
    std::cout << "\n=====================================\n";
    std::cout << "      Banking System (DS Project)    \n";
    std::cout << "=====================================\n";
    std::cout << "1. Create Account\n";
    std::cout << "2. Deposit Money (Direct)\n";
    std::cout << "3. Withdraw Money (Direct)\n";
    std::cout << "4. Add Pending Transaction (Queue)\n";
    std::cout << "5. Process Pending Queue\n";
    std::cout << "6. Show All Accounts\n";
    std::cout << "7. Show Account Summary\n";
    std::cout << "8. Show Account History\n";
    std::cout << "9. Apply Interest to All Accounts\n";
    std::cout << "10. Save Data Now\n";
    std::cout << "0. Exit\n";
    std::cout << "-------------------------------------\n";
}

} // anonymous namespace

void runMainMenu(Bank& bank) {
    int choice = -1;

    do {
        printMenu();
        choice = askInt("Enter your choice: ");

        switch (choice) {
            case 1: { // Create Account
                int accNo = askInt("Enter new account number: ");
                std::string name = askLine("Enter account holder name: ");
                double initial = askDouble("Enter initial balance: ");
                createAccount(bank, accNo, name, initial);
                waitForEnter();
                break;
            }
            case 2: { // Deposit
                int accNo = askInt("Enter account number: ");
                double amount = askDouble("Enter deposit amount: ");
                depositDirect(bank, accNo, amount);
                waitForEnter();
                break;
            }
            case 3: { // Withdraw
                int accNo = askInt("Enter account number: ");
                double amount = askDouble("Enter withdrawal amount: ");
                withdrawDirect(bank, accNo, amount);
                waitForEnter();
                break;
            }
            case 4: { // Add pending
                int accNo = askInt("Enter account number: ");
                std::cout << "Select type: 1) Deposit  2) Withdraw\n";
                int t = askInt("Your choice: ");
                TransactionType type =
                    (t == 1 ? TransactionType::Deposit : TransactionType::Withdraw);
                double amount = askDouble("Enter amount: ");
                enqueuePendingTransaction(bank, accNo, type, amount);
                waitForEnter();
                break;
            }
            case 5: { // Process queue
                processPendingQueue(bank);
                waitForEnter();
                break;
            }
            case 6: { // Show all accounts
                std::cout << "\nAll accounts:\n";
                printAllAccounts(bank);
                waitForEnter();
                break;
            }
            case 7: { // Show account summary
                int accNo = askInt("Enter account number: ");
                printAccountByNumber(bank, accNo);
                waitForEnter();
                break;
            }
            case 8: { // Show account history
                int accNo = askInt("Enter account number: ");
                printAccountHistory(bank, accNo);
                waitForEnter();
                break;
            }
            case 9: { // Apply interest
                double rate = askDouble("Enter interest rate (e.g. 0.01 for 1%): ");
                applyInterestAll(bank, rate);
                waitForEnter();
                break;
            }
            case 10: { // Save data now
                if (saveBankToFiles(bank, "accounts.csv", "transactions.csv")) {
                    std::cout << "Data saved successfully.\n";
                } else {
                    std::cout << "Failed to save data.\n";
                }
                waitForEnter();
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Unknown option. Please try again.\n";
                waitForEnter();
                break;
        }

    } while (choice != 0);
}

} // namespace bank
