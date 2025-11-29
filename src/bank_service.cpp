#include "bank_service.h"

#include <iostream>

namespace bank {

void initBank(Bank& bank) {
    bank.accountsRoot = nullptr;
    initQueue(bank.pendingQueue);
}

void destroyBank(Bank& bank) {
    freeAccountTree(bank.accountsRoot);   // frees all accounts + histories
    freeQueue(bank.pendingQueue);         // frees any remaining pending txns
    bank.accountsRoot = nullptr;
}

bool createAccount(Bank& bank,
                   int accountNumber,
                   const std::string& holderName,
                   double initialBalance) {
    if (accountNumber <= 0) {
        std::cout << "Account number must be positive.\n";
        return false;
    }
    if (initialBalance < 0.0) {
        std::cout << "Initial balance cannot be negative.\n";
        return false;
    }

    bool inserted = false;
    insertAccount(bank.accountsRoot,
                  accountNumber,
                  holderName,
                  initialBalance,
                  inserted);

    if (!inserted) {
        std::cout << "Account #" << accountNumber << " already exists.\n";
    }

    return inserted;
}

bool depositDirect(Bank& bank,
                   int accountNumber,
                   double amount) {
    if (amount <= 0.0) {
        std::cout << "Deposit amount must be positive.\n";
        return false;
    }

    AccountNode* node = searchAccount(bank.accountsRoot, accountNumber);
    if (!node) {
        std::cout << "Account #" << accountNumber << " not found.\n";
        return false;
    }

    // Update balance.
    node->data.balance += amount;

    // Record transaction.
    const std::string datetime = getCurrentDateTime();
    addTransaction(node->data.historyHead,
                   TransactionType::Deposit,
                   amount,
                   datetime);

    std::cout << "Deposited " << amount << " to account #" << accountNumber << ".\n";
    return true;
}

bool withdrawDirect(Bank& bank,
                    int accountNumber,
                    double amount) {
    if (amount <= 0.0) {
        std::cout << "Withdrawal amount must be positive.\n";
        return false;
    }

    AccountNode* node = searchAccount(bank.accountsRoot, accountNumber);
    if (!node) {
        std::cout << "Account #" << accountNumber << " not found.\n";
        return false;
    }

    if (node->data.balance < amount) {
        std::cout << "Insufficient funds in account #" << accountNumber << ".\n";
        return false;
    }

    node->data.balance -= amount;

    const std::string datetime = getCurrentDateTime();
    addTransaction(node->data.historyHead,
                   TransactionType::Withdraw,
                   amount,
                   datetime);

    std::cout << "Withdrew " << amount << " from account #" << accountNumber << ".\n";
    return true;
}

bool enqueuePendingTransaction(Bank& bank,
                               int accountNumber,
                               TransactionType type,
                               double amount) {
    if (amount <= 0.0) {
        std::cout << "Amount must be positive.\n";
        return false;
    }

    // Validate account exists before enqueueing.
    AccountNode* node = searchAccount(bank.accountsRoot, accountNumber);
    if (!node) {
        std::cout << "Account #" << accountNumber << " not found. Cannot enqueue.\n";
        return false;
    }

    if (type != TransactionType::Deposit &&
        type != TransactionType::Withdraw) {
        std::cout << "Only deposit and withdraw are allowed in queue.\n";
        return false;
    }

    enqueue(bank.pendingQueue, accountNumber, type, amount);
    std::cout << "Enqueued " << (type == TransactionType::Deposit ? "DEPOSIT" : "WITHDRAW")
              << " of " << amount << " for account #" << accountNumber << ".\n";

    return true;
}

void processPendingQueue(Bank& bank) {
    std::cout << "\nProcessing pending queue...\n";

    PendingTransaction* pt = nullptr;

    while (!isQueueEmpty(bank.pendingQueue)) {
        bool ok = dequeue(bank.pendingQueue, pt);
        if (!ok || !pt) {
            break; // safety
        }

        const std::string datetime = getCurrentDateTime();

        AccountNode* node = searchAccount(bank.accountsRoot, pt->accountNumber);
        if (!node) {
            std::cout << "Account #" << pt->accountNumber
                      << " not found. Skipping queued transaction.\n";
            delete pt;
            pt = nullptr;
            continue;
        }

        if (pt->type == TransactionType::Deposit) {
            node->data.balance += pt->amount;
            addTransaction(node->data.historyHead,
                           TransactionType::Deposit,
                           pt->amount,
                           datetime);
            std::cout << "Applied queued DEPOSIT of " << pt->amount
                      << " to account #" << pt->accountNumber << ".\n";
        } else if (pt->type == TransactionType::Withdraw) {
            if (node->data.balance < pt->amount) {
                std::cout << "Queued WITHDRAW " << pt->amount
                          << " from account #" << pt->accountNumber
                          << " skipped (insufficient funds).\n";
            } else {
                node->data.balance -= pt->amount;
                addTransaction(node->data.historyHead,
                               TransactionType::Withdraw,
                               pt->amount,
                               datetime);
                std::cout << "Applied queued WITHDRAW of " << pt->amount
                          << " from account #" << pt->accountNumber << ".\n";
            }
        }

        delete pt;
        pt = nullptr;
    }

    std::cout << "Done processing queue.\n";
}

void printAllAccounts(const Bank& bank) {
    if (!bank.accountsRoot) {
        std::cout << "(no accounts)\n";
        return;
    }
    inorderPrintAccounts(bank.accountsRoot);
}

bool printAccountByNumber(const Bank& bank,
                          int accountNumber) {
    AccountNode* node = searchAccount(bank.accountsRoot, accountNumber);
    if (!node) {
        std::cout << "Account #" << accountNumber << " not found.\n";
        return false;
    }
    printAccountSummary(node->data);
    return true;
}

bool printAccountHistory(const Bank& bank,
                         int accountNumber) {
    AccountNode* node = searchAccount(bank.accountsRoot, accountNumber);
    if (!node) {
        std::cout << "Account #" << accountNumber << " not found.\n";
        return false;
    }

    std::cout << "History for account #" << accountNumber << ":\n";
    printTransactions(node->data.historyHead);
    return true;
}

void applyInterestAll(Bank& bank, double rate) {
    if (rate <= 0.0) {
        std::cout << "Interest rate must be positive.\n";
        return;
    }

    const std::string datetime = getCurrentDateTime();

    // Recursive lambda to traverse BST and apply interest.
    std::function<void(AccountNode*)> applyRec = [&](AccountNode* node) {
        if (!node) return;

        applyRec(node->left);

        double interest = node->data.balance * rate;
        if (interest != 0.0) {
            node->data.balance += interest;
            addTransaction(node->data.historyHead,
                           TransactionType::Interest,
                           interest,
                           datetime);
        }

        applyRec(node->right);
    };

    applyRec(bank.accountsRoot);

    std::cout << "Applied interest with rate " << rate << " to all accounts.\n";
}

} // namespace bank
