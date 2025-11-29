# 🏦 Banking System (Data Structures Project – C++17)

A modular, terminal-based **Banking System** implemented in **C++17** for a Data Structures course project.  
The system models real banking behavior using classical data structures:

- **Binary Search Tree (BST)** for storing and searching accounts
- **Linked List** for transaction history
- **Queue (FIFO)** for pending transactions
- **Service Layer** for business logic
- **UI Layer** for user interaction

---

## 📘 1. Project Overview

This project simulates a small banking backend with features such as account creation, direct deposits/withdrawals, queued transactions, transaction history, and interest application.

The system is entirely console-based and focuses on designing clean architecture, proper memory management, and correct usage of data structures.

---

## 🧱 2. Project Structure

```
bankingSystem/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── utils.h
│   ├── transaction_list.h
│   ├── account_bst.h
│   ├── pending_queue.h
│   ├── bank_service.h
│   └── ui.h
└── src/
    ├── main.cpp
    ├── utils.cpp
    ├── transaction_list.cpp
    ├── account_bst.cpp
    ├── pending_queue.cpp
    ├── bank_service.cpp
    └── ui.cpp
```

Each module is fully separated with its own header and implementation file.

---

## 🧩 3. Architecture Overview

### **3.1. Data Layer**
| Component | Data Structure | Purpose |
|----------|----------------|---------|
| Account Tree | Binary Search Tree | Fast search/insert, ordered listing |
| Transaction History | Singly Linked List | Append-only history per account |
| Pending Queue | FIFO Queue | Batch processing of future transactions |

### **3.2. Service Layer**
Handles all business logic:
- createAccount
- depositDirect
- withdrawDirect
- enqueuePendingTransaction
- processPendingQueue
- printAccount / printAllAccounts
- printAccountHistory
- applyInterestAll

### **3.3. UI Layer**
- Menu-driven terminal interface
- Input validation (safe int/double/string reading)
- Invokes the service layer only

---

## 🚀 4. Features

### ✔ Account Management
- Create new accounts
- BST ensures unique account numbers
- Sorted listing of accounts

### ✔ Direct Transactions
- Deposit money
- Withdraw money with balance check
- Every operation written to transaction list with timestamp

### ✔ Pending Transactions
- Queue deposit/withdraw requests
- FIFO processing
- Accounts validated before enqueueing
- Safe handling of insufficient funds

### ✔ Reporting
- Show all accounts
- Show a single account summary
- Show full transaction history
- Apply interest to all accounts

---

## 🔧 5. Build & Run Instructions

### **Option A — Using CLion (Recommended)**

1. Open project in CLion
2. CLion auto-detects `CMakeLists.txt`
3. Reload CMake
4. Click **Build**
5. Run the executable

### **Option B — Using Terminal with CMake**
(Requires CMake installed via Homebrew or Xcode tools)

```bash
cd bankingSystem
cmake -S . -B build
cmake --build build
./build/bankingSystem
