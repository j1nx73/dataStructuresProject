# Banking System (C++ • Data Structures Project)

A simple banking system implemented in C++ to demonstrate core data structures. Accounts are stored in a Binary Search Tree, each account maintains a linked-list transaction history, and pending operations are handled through a queue.

## Features
- Create accounts with initial balance  
- Direct deposits and withdrawals  
- Queued transactions (deposit/withdraw)  
- Process pending transactions in batch  
- View account details and transaction history  
- CLI interface  
- Money stored as integer cents for precision  

## Data Structures Used
- **BST** — account storage and search  
- **Linked List** — per-account transaction log  
- **Queue** — pending operations  

## Build & Run
```bash
g++ -std=c++17 main.cpp -o bank
./bank
