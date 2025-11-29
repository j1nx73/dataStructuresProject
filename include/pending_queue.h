#ifndef PENDING_QUEUE_H
#define PENDING_QUEUE_H

#include "transaction_list.h"  // for TransactionType
#include <string>

namespace bank {

/// One pending transaction in the daily processing queue.
///
/// Fields:
///  - accountNumber : ID of the account to which this applies
///  - type          : deposit or withdraw (we could also allow Interest)
///  - amount        : money amount to apply
///  - next          : pointer to the next node in the queue
struct PendingTransaction {
    int accountNumber{};
    TransactionType type{};
    double amount{};
    PendingTransaction* next{nullptr};

    PendingTransaction(int accNo,
                       TransactionType t,
                       double amt)
        : accountNumber(accNo),
          type(t),
          amount(amt),
          next(nullptr) {}
};

/// Simple FIFO queue implemented as a linked list.
///
/// Fields:
///  - front : pointer to the first element (oldest)
///  - back  : pointer to the last element (newest)
struct PendingQueue {
    PendingTransaction* front{nullptr};
    PendingTransaction* back{nullptr};
};

/// Initializes the queue to an empty state.
void initQueue(PendingQueue& q);

/// Returns true if the queue has no elements.
bool isQueueEmpty(const PendingQueue& q);

/// Adds a new pending transaction to the back of the queue.
void enqueue(PendingQueue& q,
             int accountNumber,
             TransactionType type,
             double amount);

/// Removes the oldest pending transaction from the front of the queue.
///
/// @param q   Queue to dequeue from.
/// @param out Pointer reference that will receive the removed node
///            (caller must `delete` it if true is returned).
///
/// @return true  if a node was dequeued and `out` now points to it.
///         false if the queue was empty and `out` is set to nullptr.
bool dequeue(PendingQueue& q, PendingTransaction*& out);

/// Frees all nodes in the queue and resets it to empty.
void freeQueue(PendingQueue& q);

/// Counts how many elements are currently in the queue.
int queueSize(const PendingQueue& q);

} // namespace bank

#endif // PENDING_QUEUE_H
