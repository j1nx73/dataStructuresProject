#include "account_bst.h"

#include <iostream>

namespace bank {

AccountNode* insertAccount(AccountNode*& root,
                           int accountNumber,
                           const std::string& name,
                           double initialBalance,
                           bool& inserted) {
    // If tree (subtree) is empty, create a new node here.
    if (root == nullptr) {
        Account newAcc(accountNumber, name, initialBalance);
        root = new AccountNode(newAcc);
        inserted = true;
        return root;
    }

    // If the key already exists, do not insert a duplicate.
    if (accountNumber == root->data.accountNumber) {
        inserted = false;
        return root; // return existing node
    }

    // If new account number is smaller, go to left subtree.
    if (accountNumber < root->data.accountNumber) {
        return insertAccount(root->left, accountNumber, name, initialBalance, inserted);
    } else {
        // Otherwise, go to right subtree.
        return insertAccount(root->right, accountNumber, name, initialBalance, inserted);
    }
}

AccountNode* searchAccount(AccountNode* root, int accountNumber) {
    AccountNode* current = root;

    // Classic BST iterative search.
    while (current != nullptr) {
        if (accountNumber == current->data.accountNumber) {
            return current; // found it
        } else if (accountNumber < current->data.accountNumber) {
            current = current->left; // go left
        } else {
            current = current->right; // go right
        }
    }

    // Reached a null pointer: not found.
    return nullptr;
}

void printAccountSummary(const Account& account) {
    std::cout << "Account #" << account.accountNumber
              << " | Holder: " << account.holderName
              << " | Balance: " << account.balance
              << '\n';
}

void inorderPrintAccounts(const AccountNode* root) {
    if (root == nullptr) {
        return; // base case: empty subtree
    }

    // 1) Print left subtree
    inorderPrintAccounts(root->left);

    // 2) Print current node
    printAccountSummary(root->data);

    // 3) Print right subtree
    inorderPrintAccounts(root->right);
}

void freeAccountTree(AccountNode*& root) {
    if (root == nullptr) {
        return;
    }

    // Post-order traversal: free children first.
    freeAccountTree(root->left);
    freeAccountTree(root->right);

    // Free the transaction history list for this account.
    freeTransactions(root->data.historyHead);

    // Then free the node itself.
    delete root;
    root = nullptr;
}

} // namespace bank
