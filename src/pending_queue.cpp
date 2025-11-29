#include "pending_queue.h"

#include <iostream>

namespace bank {

void initQueue(PendingQueue& q) {
    // Set both pointers to nullptr to represent an empty queue.
    q.front = nullptr;
    q.back  = nullptr;
}

bool isQueueEmpty(const PendingQueue& q) {
    // Queue is empty when there is no front node.
    return q.front == nullptr;
}

void enqueue(PendingQueue& q,
             int accountNumber,
             TransactionType type,
             double amount) {
    // 1) Allocate a new node with given data.
    PendingTransaction* node = new PendingTransaction(accountNumber, type, amount);

    // 2) If queue is empty, this node becomes both front and back.
    if (q.back == nullptr) {
        q.front = node;
        q.back  = node;
        return;
    }

    // 3) Otherwise, link it after the current back and update back.
    q.back->next = node;
    q.back = node;
}

bool dequeue(PendingQueue& q, PendingTransaction*& out) {
    // If queue is empty, nothing to remove.
    if (q.front == nullptr) {
        out = nullptr;
        return false;
    }

    // 1) Take the front node.
    PendingTransaction* node = q.front;

    // 2) Move front pointer to the next node.
    q.front = q.front->next;

    // 3) If front became nullptr, the queue is now empty; adjust back as well.
    if (q.front == nullptr) {
        q.back = nullptr;
    }

    // 4) Give the removed node to the caller.
    out = node;
    return true;
}

void freeQueue(PendingQueue& q) {
    PendingTransaction* current = q.front;

    // Iterate through all nodes and delete them.
    while (current != nullptr) {
        PendingTransaction* next = current->next;
        delete current;
        current = next;
    }

    // Reset queue to empty state.
    q.front = nullptr;
    q.back  = nullptr;
}

int queueSize(const PendingQueue& q) {
    int count = 0;
    PendingTransaction* current = q.front;

    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

} // namespace bank
