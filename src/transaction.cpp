#include "transaction.h"

#include <iostream>
#include <iomanip>

// Append a node at the tail of the singly linked list
void appendTransaction(Transaction*& head,
                       Transaction*& tail,
                       TransactionType type,
                       double amount,
                       const std::string& note,
                       const std::string& datetime)
{
    Transaction* node = new Transaction(type, amount, note, datetime);

    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        tail       = node;
    }
}

// Print all transactions in the linked list
void printTransactionHistory(const Transaction* head)
{
    if (!head) {
        std::cout << "  No transactions.\n";
        return;
    }

    const Transaction* curr = head;
    while (curr) {
        std::cout << "  [" << curr->datetime << "] "
                  << std::left << std::setw(12) << toString(curr->type)
                  << " Amount: " << curr->amount
                  << " | Note: " << curr->note << "\n";
        curr = curr->next;
    }
}

// Free transaction linked list to avoid memory leaks
void freeTransactionList(Transaction*& head, Transaction*& tail)
{
    Transaction* curr = head;
    while (curr) {
        Transaction* next = curr->next;
        delete curr;
        curr = next;
    }
    head = nullptr;
    tail = nullptr;
}

// Helper for printing transaction type
std::string toString(TransactionType type)
{
    switch (type) {
        case TransactionType::Deposit:       return "DEPOSIT";
        case TransactionType::Withdraw:      return "WITHDRAW";
        case TransactionType::Interest:      return "INTEREST";
        case TransactionType::QueueDeposit:  return "Q-DEPOSIT";
        case TransactionType::QueueWithdraw: return "Q-WITHDRAW";
    }
    return "UNKNOWN";
}
