#ifndef ACCOUNT_BST_H
#define ACCOUNT_BST_H

#include <string>
#include "transaction_list.h"

namespace bank {

/// Represents one bank account.
///
/// Fields:
///  - accountNumber : unique integer ID (key in the BST)
///  - holderName    : owner's name
///  - balance       : current money balance
///  - historyHead   : pointer to the head of the transaction list
struct Account {
    int         accountNumber{};
    std::string holderName;
    double      balance{};
    Transaction* historyHead{nullptr};

    /// Convenience constructor to initialize all fields.
    Account(int number = 0,
            std::string name = {},
            double bal = 0.0)
        : accountNumber(number),
          holderName(std::move(name)),
          balance(bal),
          historyHead(nullptr) {}
};

/// Node in the Binary Search Tree of accounts.
///
/// Each node stores:
///  - data : the Account
///  - left : pointer to left child (accounts with smaller accountNumber)
///  - right: pointer to right child (accounts with larger accountNumber)
struct AccountNode {
    Account     data;
    AccountNode* left;
    AccountNode* right;

    explicit AccountNode(const Account& acc)
        : data(acc), left(nullptr), right(nullptr) {}
};

/// Inserts a new account into the BST rooted at `root`.
///
/// @param root          Reference to pointer of tree root.
/// @param accountNumber New account's unique ID.
/// @param name          Account holder name.
/// @param initialBalance Starting balance.
/// @param inserted      Output flag:
///                      - true  if a new node was inserted
///                      - false if an account with this number already exists
///
/// @return Pointer to the node (existing or newly created).
AccountNode* insertAccount(AccountNode*& root,
                           int accountNumber,
                           const std::string& name,
                           double initialBalance,
                           bool& inserted);

/// Searches the BST for an account by accountNumber.
///
/// @param root          Pointer to tree root.
/// @param accountNumber ID we are looking for.
/// @return              Pointer to the node if found, nullptr otherwise.
AccountNode* searchAccount(AccountNode* root, int accountNumber);

/// Prints a one-line summary of a single account.
void printAccountSummary(const Account& account);

/// Performs in-order traversal of the BST and prints each account.
///
/// This prints accounts sorted by accountNumber.
void inorderPrintAccounts(const AccountNode* root);

/// Frees the entire BST and all associated transaction lists.
///
/// After this call, root is set to nullptr.
void freeAccountTree(AccountNode*& root);

} // namespace bank

#endif // ACCOUNT_BST_H
