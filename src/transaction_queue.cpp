#include "transaction_queue.h"
#include <iostream>

TransactionQueue::TransactionQueue()
    : front(nullptr),
      rear(nullptr)
{}

TransactionQueue::~TransactionQueue()
{
    clear();
}

// Enqueue new transaction at the tail
void TransactionQueue::enqueue(int accNo,
                               TransactionType type,
                               double amount,
                               const std::string& note)
{
    PendingTransaction* node =
        new PendingTransaction(accNo, type, amount, note);

    if (!rear) {
        front = rear = node;
    } else {
        rear->next = node;
        rear       = node;
    }
}

// Dequeue from head (O(1))
bool TransactionQueue::dequeue(PendingTransaction*& out)
{
    if (!front) {
        out = nullptr;
        return false;
    }

    PendingTransaction* node = front;
    front = front->next;
    if (!front) {
        rear = nullptr;
    }

    node->next = nullptr;
    out = node;
    return true;
}

bool TransactionQueue::isEmpty() const
{
    return front == nullptr;
}

void TransactionQueue::clear()
{
    PendingTransaction* curr = front;
    while (curr) {
        PendingTransaction* next = curr->next;
        delete curr;
        curr = next;
    }
    front = rear = nullptr;
}
