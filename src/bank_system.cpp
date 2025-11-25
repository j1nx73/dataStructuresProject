#include "bank_system.h"

#include <iostream>
#include <limits>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Helper: current date-time as string
std::string BankSystem::currentDateTimeString()
{
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t tt = clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Print menu options
void BankSystem::showMenu() const
{
    std::cout << "\n=== Banking System Menu ===\n";
    std::cout << "1. Create account\n";
    std::cout << "2. Direct deposit\n";
    std::cout << "3. Direct withdraw\n";
    std::cout << "4. Queue deposit\n";
    std::cout << "5. Queue withdraw\n";
    std::cout << "6. Process all pending transactions\n";
    std::cout << "7. Display account details + history\n";
    std::cout << "8. Display all accounts (in order)\n";
    std::cout << "9. Apply interest to all accounts\n";
    std::cout << "10. Show bank statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

// Main interactive loop
void BankSystem::run()
{
    int choice = -1;
    while (true) {
        showMenu();
        if (!(std::cin >> choice)) {
            // Input error – clear and ignore line
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:  handleCreateAccount();    break;
            case 2:  handleDirectDeposit();    break;
            case 3:  handleDirectWithdraw();   break;
            case 4:  handleQueueDeposit();     break;
            case 5:  handleQueueWithdraw();    break;
            case 6:  handleProcessQueue();     break;
            case 7:  handleShowAccount();      break;
            case 8:  handleShowAllAccounts();  break;
            case 9:  handleApplyInterest();    break;
            case 10: handleStatistics();       break;
            case 0:
                std::cout << "Exiting program. Goodbye!\n";
                return;
            default:
                std::cout << "Unknown choice. Please try again.\n";
        }
    }
}

// ------------- Menu handlers -------------

void BankSystem::handleCreateAccount()
{
    int accNo;
    std::string name;
    double initialBalance;

    std::cout << "Enter new account number: ";
    std::cin  >> accNo;

    std::cout << "Enter holder name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    std::cout << "Enter initial balance: ";
    std::cin  >> initialBalance;

    accounts.insert(accNo, name, initialBalance);
    std::cout << "Account created.\n";
}

void BankSystem::handleDirectDeposit()
{
    int accNo;
    double amount;

    std::cout << "Enter account number: ";
    std::cin  >> accNo;
    std::cout << "Enter deposit amount: ";
    std::cin  >> amount;

    if (amount <= 0) {
        std::cout << "Deposit must be positive.\n";
        return;
    }

    AccountNode* acc = accounts.find(accNo);
    if (!acc) {
        std::cout << "Account not found.\n";
        return;
    }

    acc->balance += amount;
    appendTransaction(acc->historyHead,
                      acc->historyTail,
                      TransactionType::Deposit,
                      amount,
                      "Direct deposit",
                      currentDateTimeString());

    std::cout << "Deposit successful. New balance: " << acc->balance << "\n";
}

void BankSystem::handleDirectWithdraw()
{
    int accNo;
    double amount;

    std::cout << "Enter account number: ";
    std::cin  >> accNo;
    std::cout << "Enter withdraw amount: ";
    std::cin  >> amount;

    if (amount <= 0) {
        std::cout << "Withdraw amount must be positive.\n";
        return;
    }

    AccountNode* acc = accounts.find(accNo);
    if (!acc) {
        std::cout << "Account not found.\n";
        return;
    }

    if (amount > acc->balance) {
        std::cout << "Insufficient balance.\n";
        return;
    }

    acc->balance -= amount;
    appendTransaction(acc->historyHead,
                      acc->historyTail,
                      TransactionType::Withdraw,
                      amount,
                      "Direct withdraw",
                      currentDateTimeString());

    std::cout << "Withdraw successful. New balance: " << acc->balance << "\n";
}

void BankSystem::handleQueueDeposit()
{
    int accNo;
    double amount;

    std::cout << "Enter account number: ";
    std::cin  >> accNo;
    std::cout << "Enter deposit amount to queue: ";
    std::cin  >> amount;

    if (amount <= 0) {
        std::cout << "Amount must be positive.\n";
        return;
    }

    // We can optionally check that account exists now, or let processing time handle it.
    pending.enqueue(accNo,
                    TransactionType::Deposit,
                    amount,
                    "Queued deposit");
    std::cout << "Deposit queued.\n";
}

void BankSystem::handleQueueWithdraw()
{
    int accNo;
    double amount;

    std::cout << "Enter account number: ";
    std::cin  >> accNo;
    std::cout << "Enter withdraw amount to queue: ";
    std::cin  >> amount;

    if (amount <= 0) {
        std::cout << "Amount must be positive.\n";
        return;
    }

    pending.enqueue(accNo,
                    TransactionType::Withdraw,
                    amount,
                    "Queued withdraw");
    std::cout << "Withdraw queued.\n";
}

// Process all transactions from the queue (FIFO)
void BankSystem::handleProcessQueue()
{
    if (pending.isEmpty()) {
        std::cout << "No pending transactions.\n";
        return;
    }

    std::cout << "Processing pending transactions...\n";

    PendingTransaction* t = nullptr;
    while (pending.dequeue(t)) {
        if (!t) continue;

        AccountNode* acc = accounts.find(t->accountNumber);
        if (!acc) {
            std::cout << "[QUEUE] Account " << t->accountNumber
                      << " not found, transaction skipped.\n";
            delete t;
            continue;
        }

        std::string when = currentDateTimeString();

        if (t->type == TransactionType::Deposit) {
            acc->balance += t->amount;
            appendTransaction(acc->historyHead,
                              acc->historyTail,
                              TransactionType::Deposit,
                              t->amount,
                              "Processed queue: " + t->note,
                              when);
            std::cout << "[QUEUE] Deposit " << t->amount
                      << " applied to account " << acc->accountNumber << ".\n";
        } else if (t->type == TransactionType::Withdraw) {
            if (t->amount > acc->balance) {
                std::cout << "[QUEUE] Insufficient funds for account "
                          << acc->accountNumber << ", withdraw skipped.\n";
            } else {
                acc->balance -= t->amount;
                appendTransaction(acc->historyHead,
                                  acc->historyTail,
                                  TransactionType::Withdraw,
                                  t->amount,
                                  "Processed queue: " + t->note,
                                  when);
                std::cout << "[QUEUE] Withdraw " << t->amount
                          << " applied to account " << acc->accountNumber << ".\n";
            }
        }

        delete t; // free queue node
    }

    std::cout << "All pending transactions processed.\n";
}

void BankSystem::handleShowAccount() const
{
    int accNo;
    std::cout << "Enter account number: ";
    std::cin  >> accNo;
    accounts.printAccountDetails(accNo);
}

void BankSystem::handleShowAllAccounts() const
{
    accounts.printInOrder();
}

void BankSystem::handleApplyInterest()
{
    double rate;
    int mode;

    std::cout << "Enter annual interest rate (e.g., 0.12 for 12%): ";
    std::cin  >> rate;
    std::cout << "Apply (1) monthly or (2) yearly interest? ";
    std::cin  >> mode;

    bool monthly = (mode == 1);
    accounts.applyInterest(rate, monthly, currentDateTimeString());
}

void BankSystem::handleStatistics() const
{
    accounts.statistics();
}
