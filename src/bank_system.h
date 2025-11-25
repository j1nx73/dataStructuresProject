#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include "account.h"
#include "transaction_queue.h"

// High-level class that glues together BST, linked list, and queue
class BankSystem {
public:
    // Entry point for interactive mode
    void run();

private:
    AccountBST       accounts;
    TransactionQueue pending;

    void showMenu() const;

    void handleCreateAccount();
    void handleDirectDeposit();
    void handleDirectWithdraw();
    void handleQueueDeposit();
    void handleQueueWithdraw();
    void handleProcessQueue();
    void handleShowAccount() const;
    void handleShowAllAccounts() const;
    void handleApplyInterest();
    void handleStatistics() const;

    static std::string currentDateTimeString();
};

#endif // BANK_SYSTEM_H
