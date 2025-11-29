#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include <string>
#include <iosfwd> // forward declarations for std::ostream if needed

namespace bank {

/// Represents the type of a bank transaction.
///
/// Deposit  - money is added to the account
/// Withdraw - money is removed from the account
/// Interest - interest is added to the account (optional feature)
enum class TransactionType {
    Deposit,
    Withdraw,
    Interest
};

/// Node for a singly linked list storing one transaction.
///
/// Fields:
///  - type:     deposit / withdraw / interest
///  - amount:   numeric value of the operation
///  - datetime: timestamp string "YYYY-MM-DD HH:MM:SS"
///  - next:     pointer to the next node in the history list
struct Transaction {
    TransactionType type;
    double amount;
    std::string datetime;
    Transaction* next;

    /// Convenience constructor to initialize all fields at once.
    Transaction(TransactionType t,
                double a,
                const std::string& dt,
                Transaction* n = nullptr)
        : type(t), amount(a), datetime(dt), next(n) {}
};

/// Appends a new transaction node to the end of the list.
///
/// @param head   Reference to the head pointer of the list.
///               This may change if the list was previously empty.
/// @param type   Transaction type (Deposit / Withdraw / Interest).
/// @param amount Transaction amount.
/// @param datetime Timestamp string for when this transaction happened.
///
/// Internally, this function allocates a new Transaction using `new`
/// and either:
///  - sets head to point to it (if list was empty), or
///  - links it after the current last node.
void addTransaction(Transaction*& head,
                    TransactionType type,
                    double amount,
                    const std::string& datetime);

/// Prints all transactions in the list to std::cout.
///
/// Format:
///   index) TYPE: amount on datetime
///
/// If the list is empty, prints "(no transactions)".
void printTransactions(const Transaction* head);

/// Frees all nodes in the transaction list and sets head to nullptr.
///
/// This prevents memory leaks when we are done with the history.
void freeTransactions(Transaction*& head);

/// Counts the number of nodes (transactions) in the list.
///
/// @return Non-negative integer count.
int countTransactions(const Transaction* head);

/// Returns a pointer to the last transaction node in the list,
/// or nullptr if the list is empty.
Transaction* getLastTransaction(Transaction* head);

} // namespace bank

#endif // TRANSACTION_LIST_H
