#include "transaction_list.h"

#include <iostream>  // std::cout
#include <stdexcept> // optional, if you want to handle exceptions

namespace bank {

/// Helper function (local to this file) that converts
/// TransactionType enum to a human-readable string.
static const char* toString(TransactionType type) {
    switch (type) {
        case TransactionType::Deposit:  return "Deposit";
        case TransactionType::Withdraw: return "Withdrawal";
        case TransactionType::Interest: return "Interest";
        default:                        return "Unknown";
    }
}

void addTransaction(Transaction*& head,
                    TransactionType type,
                    double amount,
                    const std::string& datetime) {
    // 1) Dynamically allocate a new Transaction node.
    //
    //    We use the Transaction constructor to initialize all fields.
    Transaction* newNode = new Transaction(type, amount, datetime, nullptr);

    // 2) If the list is currently empty, newNode becomes the head.
    if (head == nullptr) {
        head = newNode;
        return;
    }

    // 3) Otherwise, iterate to the end of the list.
    Transaction* current = head;

    // Loop invariant: current points to a valid node in the list.
    while (current->next != nullptr) {
        current = current->next; // move forward one node
    }

    // 4) current now points to the last node; attach newNode after it.
    current->next = newNode;
}

void printTransactions(const Transaction* head) {
    const Transaction* current = head;
    int index = 1; // human-friendly index starting from 1

    // Traverse the list from head to end.
    while (current != nullptr) {
        // Convert enum to readable string using helper function.
        const char* typeStr = toString(current->type);

        std::cout << index << ") "
                  << typeStr << ": "
                  << current->amount << " on "
                  << current->datetime << '\n';

        current = current->next; // move to next node
        ++index;                 // increment index
    }

    // If index stayed 1, the loop never executed => list was empty.
    if (index == 1) {
        std::cout << "(no transactions)\n";
    }
}

void freeTransactions(Transaction*& head) {
    // current will move through the list, deleting nodes one by one.
    Transaction* current = head;

    // Loop through the list until we reach nullptr.
    while (current != nullptr) {
        // Save pointer to the next node before deleting current.
        Transaction* next = current->next;

        // Release memory for current node.
        delete current;

        // Move forward to the next node.
        current = next;
    }

    // After the loop, the list is empty; set head to nullptr.
    head = nullptr;
}

int countTransactions(const Transaction* head) {
    int count = 0;
    const Transaction* current = head;

    // Basic traversal: visit each node and increment counter.
    while (current != nullptr) {
        ++count;
        current = current->next;
    }

    return count;
}

Transaction* getLastTransaction(Transaction* head) {
    // Empty list has no last node.
    if (head == nullptr) {
        return nullptr;
    }

    Transaction* current = head;

    // Move forward until we find node whose next is nullptr.
    while (current->next != nullptr) {
        current = current->next;
    }

    // current now points to the last node.
    return current;
}

} // namespace bank
