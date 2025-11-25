#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// High-level type of a transaction
enum class TransactionType {
    Deposit,
    Withdraw,
    Interest,
    QueueDeposit,
    QueueWithdraw
};

// Node of a singly linked list representing a single transaction
struct Transaction {
    TransactionType type;
    double          amount;
    std::string     note;      // short description
    std::string     datetime;  // timestamp as string
    Transaction*    next;

    Transaction(TransactionType t,
                double          a,
                const std::string& n,
                const std::string& dt)
        : type(t), amount(a), note(n), datetime(dt), next(nullptr) {}
};

// Append a new transaction node at the end of the list (O(1) with tail)
void appendTransaction(Transaction*& head,
                       Transaction*& tail,
                       TransactionType type,
                       double amount,
                       const std::string& note,
                       const std::string& datetime);

// Print the linked list of transactions in order
void printTransactionHistory(const Transaction* head);

// Free the entire linked list and set head (and tail) to nullptr
void freeTransactionList(Transaction*& head, Transaction*& tail);

// Convert TransactionType to human-readable string
std::string toString(TransactionType type);

#endif // TRANSACTION_H
