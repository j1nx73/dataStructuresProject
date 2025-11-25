#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "transaction.h"

// Node of the Binary Search Tree (BST)
// Each account has its own transaction history (linked list)
struct AccountNode {
    int          accountNumber;
    std::string  holderName;
    double       balance;

    Transaction* historyHead;
    Transaction* historyTail;

    AccountNode* left;
    AccountNode* right;

    AccountNode(int accNo,
                const std::string& name,
                double initBalance);
};

// Wrapper around the BST root, with convenience methods
class AccountBST {
public:
    AccountBST();
    ~AccountBST();

    // Insert a new account into the BST (returns pointer to the created account)
    AccountNode* insert(int accNo,
                        const std::string& name,
                        double initBalance);

    // Find an account by account number (nullptr if not found)
    AccountNode* find(int accNo) const;

    // Print all accounts in ascending order by account number (inorder traversal)
    void printInOrder() const;

    // Print details and transaction history for a specific account
    void printAccountDetails(int accNo) const;

    // Apply interest to every account
    // annualRate: e.g. 0.12 for 12%
    // monthly   : if true, apply monthly; otherwise yearly
    void applyInterest(double annualRate,
                       bool   monthly,
                       const std::string& datetime);

    // Print basic statistics (count, total balance, richest account)
    void statistics() const;

private:
    AccountNode* root;

    // Internal helpers
    static AccountNode* insert(AccountNode* node, AccountNode* newNode);
    static AccountNode* find(AccountNode* node, int accNo);
    static void printInOrder(const AccountNode* node);
    static void destroy(AccountNode* node);
    static void printAccountDetails(const AccountNode* node);
    static void applyInterest(AccountNode* node,
                              double annualRate,
                              bool   monthly,
                              const std::string& datetime);
    static void statistics(const AccountNode* node,
                           int& count,
                           double& totalBalance,
                           const AccountNode*& richest);
};

#endif // ACCOUNT_H
