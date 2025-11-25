#ifndef TRANSACTION_QUEUE_H
#define TRANSACTION_QUEUE_H

#include <string>
#include "transaction.h"

// Node for a linked-list-based queue
struct PendingTransaction {
    int             accountNumber;
    TransactionType type;
    double          amount;
    std::string     note;
    PendingTransaction* next;

    PendingTransaction(int accNo,
                       TransactionType t,
                       double amt,
                       const std::string& n)
        : accountNumber(accNo),
          type(t),
          amount(amt),
          note(n),
          next(nullptr)
    {}
};

// Simple FIFO queue: enqueue at rear, dequeue from front
class TransactionQueue {
public:
    TransactionQueue();
    ~TransactionQueue();

    void enqueue(int accNo,
                 TransactionType type,
                 double amount,
                 const std::string& note);

    // Dequeue a transaction; returns false if queue was empty
    bool dequeue(PendingTransaction*& out);

    bool isEmpty() const;

    // Remove all remaining nodes
    void clear();

private:
    PendingTransaction* front;
    PendingTransaction* rear;
};

#endif // TRANSACTION_QUEUE_H
