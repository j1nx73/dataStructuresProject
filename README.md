# Banking System – Data Structures Project (C++17)

## Overview

This project is a console-based **Banking System** that demonstrates the use of several fundamental data structures:

- **Binary Search Tree (BST)** – stores bank accounts by account number
- **Singly Linked List** – maintains a transaction history for each account
- **Queue (FIFO)** – stores pending transactions that can be processed in batch

The goal is to show understanding of how these data structures work **individually** and **together** in a realistic scenario.

---

## Features

1. **Account Management (BST)**
    - Create new accounts with:
        - account number
        - holder name
        - initial balance
    - Store accounts as nodes of a binary search tree
    - Inorder traversal prints accounts in sorted order by account number

2. **Transactions (Linked List)**
    - Each account has its own linked list of transactions:
        - Direct deposit
        - Direct withdrawal
        - Interest credit
        - Transactions processed from the queue
    - For every transaction we store:
        - type (deposit / withdraw / interest / etc.)
        - amount
        - timestamp
        - short note
    - Appending is done at the **tail** of the list → `O(1)` insertion

3. **Pending Transactions (Queue)**
    - Deposit or withdraw requests can be **queued** instead of processed immediately
    - Implemented as a linked-list-based FIFO queue
    - Menu option to process all queued transactions in order:
        - dequeue one by one
        - locate the account via BST
        - apply deposit/withdraw
        - record in transaction history

4. **Interest & Statistics**
    - Apply interest to all accounts (monthly or yearly)
    - For each account, interest is:
        - calculated from current balance
        - added to balance
        - recorded as an `INTEREST` transaction
    - Compute and display:
        - number of accounts
        - total balance in the bank
        - average balance
        - richest account (max balance)

5. **Interactive Menu**
    - Create account
    - Direct deposit
    - Direct withdraw
    - Queue deposit
    - Queue withdraw
    - Process all pending transactions
    - Display details & history for one account
    - Display all accounts (inorder traversal)
    - Apply interest
    - Show bank statistics
    - Exit

---

## File Structure

All files can live in a single directory:

- `transaction.h` – transaction type enum and singly linked list node
- `transaction.cpp` – functions to append, print, and free transaction lists
- `account.h` – `AccountNode` struct and `AccountBST` class (BST implementation)
- `account.cpp` – BST insert/search/traversal, interest, and statistics
- `transaction_queue.h` – `PendingTransaction` struct and `TransactionQueue` class (queue)
- `transaction_queue.cpp` – queue operations: enqueue, dequeue, clear
- `bank_system.h` – `BankSystem` class tying everything together & defining menu handlers
- `bank_system.cpp` – menu loop and all high-level operations
- `main.cpp` – entry point that creates `BankSystem` and calls `run()`

> If you want a more “industrial” layout, you can create `include/` and `src/` folders and adjust include paths accordingly.

---

## Build Instructions

### Using `g++`

Make sure you have a C++17-compatible compiler (e.g., `g++`, `clang++`).

```bash
# In the project directory:
g++ -std=c++17 main.cpp bank_system.cpp account.cpp transaction.cpp transaction_queue.cpp -o bank_system
