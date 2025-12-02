#ifndef BANK_SERVICE_H
#define BANK_SERVICE_H

#include <string>

#include "account_bst.h"
#include "pending_queue.h"
#include "transaction_list.h"
#include "utils.h"

namespace bank {

/// Aggregates all core data structures for the banking system.
struct Bank {
    AccountNode*   accountsRoot{nullptr};  // BST of accounts
    PendingQueue   pendingQueue;           // queue of pending txns
};

/// Initializes the Bank: empty BST + empty queue.
void initBank(Bank& bank);

/// Frees all accounts (and their histories) and all pending transactions.
void destroyBank(Bank& bank);

/// Creates a new account if the accountNumber is not already used.
/// @return true if inserted, false if duplicate.
bool createAccount(Bank& bank,
                   int accountNumber,
                   const std::string& holderName,
                   double initialBalance);

/// Performs a direct deposit on an existing account.
/// Adds a transaction with current datetime.
/// @return true on success, false if account not found or amount invalid.
bool depositDirect(Bank& bank,
                   int accountNumber,
                   double amount);

/// Performs a direct withdrawal on an existing account.
/// Adds a transaction with current datetime if successful.
/// @return true on success, false if account not found / invalid amount / insufficient funds.
bool withdrawDirect(Bank& bank,
                    int accountNumber,
                    double amount);

/// Adds a transaction to the pending queue (to be processed later).
/// Validates amount > 0 and that the account exists.
/// @return true if enqueued, false if validation fails.
bool enqueuePendingTransaction(Bank& bank,
                               int accountNumber,
                               TransactionType type,
                               double amount);

/// Processes all pending transactions in FIFO order.
/// For each successful operation, updates balance and adds a history record.
void processPendingQueue(Bank& bank);

/// Prints a summary of all accounts (in-order traversal of BST).
void printAllAccounts(const Bank& bank);

/// Prints a single account summary by number.
/// @return true if found and printed, false if not found.
bool printAccountByNumber(const Bank& bank,
                          int accountNumber);

/// Prints full transaction history for a given account.
/// @return true if account found, false otherwise.
bool printAccountHistory(const Bank& bank,
                         int accountNumber);

/// Interest feature: apply a simple interest rate to all accounts.
/// Example: rate = 0.01 means +1% of current balance.
/// A transaction of type Interest is added for each account.
void applyInterestAll(Bank& bank, double rate);

} // namespace bank

#endif // BANK_SERVICE_H
