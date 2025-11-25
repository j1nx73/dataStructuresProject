#include "account.h"
#include <iostream>
#include <iomanip>

// ---------------- AccountNode implementation ----------------

AccountNode::AccountNode(int accNo,
                         const std::string& name,
                         double initBalance)
    : accountNumber(accNo),
      holderName(name),
      balance(initBalance),
      historyHead(nullptr),
      historyTail(nullptr),
      left(nullptr),
      right(nullptr)
{}

// ---------------- AccountBST implementation ----------------

AccountBST::AccountBST()
    : root(nullptr)
{}

AccountBST::~AccountBST()
{
    destroy(root);
    root = nullptr;
}

// Recursively free tree and all per-account transaction lists
void AccountBST::destroy(AccountNode* node)
{
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    freeTransactionList(node->historyHead, node->historyTail);
    delete node;
}

AccountNode* AccountBST::insert(int accNo,
                                const std::string& name,
                                double initBalance)
{
    AccountNode* newNode = new AccountNode(accNo, name, initBalance);
    root = insert(root, newNode);
    return find(accNo);
}

// Recursive BST insert helper
AccountNode* AccountBST::insert(AccountNode* node, AccountNode* newNode)
{
    if (!node) {
        return newNode;
    }

    if (newNode->accountNumber < node->accountNumber) {
        node->left = insert(node->left, newNode);
    } else if (newNode->accountNumber > node->accountNumber) {
        node->right = insert(node->right, newNode);
    } else {
        // Duplicate account number – keep existing, discard new
        std::cerr << "Warning: account " << newNode->accountNumber
                  << " already exists, ignoring duplicate.\n";
        delete newNode;
    }
    return node;
}

// Public lookup wrapper
AccountNode* AccountBST::find(int accNo) const
{
    return find(root, accNo);
}

// Iterative BST search
AccountNode* AccountBST::find(AccountNode* node, int accNo)
{
    while (node) {
        if (accNo == node->accountNumber) {
            return node;
        } else if (accNo < node->accountNumber) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return nullptr;
}

void AccountBST::printInOrder() const
{
    if (!root) {
        std::cout << "No accounts.\n";
        return;
    }
    printInOrder(root);
}

void AccountBST::printInOrder(const AccountNode* node)
{
    if (!node) return;
    printInOrder(node->left);

    std::cout << "Account #" << node->accountNumber
              << " | Name: " << node->holderName
              << " | Balance: " << node->balance << "\n";

    printInOrder(node->right);
}

void AccountBST::printAccountDetails(int accNo) const
{
    AccountNode* node = find(accNo);
    if (!node) {
        std::cout << "Account " << accNo << " not found.\n";
        return;
    }
    printAccountDetails(node);
}

void AccountBST::printAccountDetails(const AccountNode* node)
{
    if (!node) return;

    std::cout << "------------------------------\n";
    std::cout << "Account number : " << node->accountNumber << "\n";
    std::cout << "Holder name    : " << node->holderName << "\n";
    std::cout << "Balance        : " << node->balance << "\n";
    std::cout << "Transaction history:\n";
    printTransactionHistory(node->historyHead);
    std::cout << "------------------------------\n";
}

// Apply interest to every account using inorder traversal
void AccountBST::applyInterest(double annualRate,
                               bool   monthly,
                               const std::string& datetime)
{
    if (!root) {
        std::cout << "No accounts to apply interest.\n";
        return;
    }
    applyInterest(root, annualRate, monthly, datetime);
}

void AccountBST::applyInterest(AccountNode* node,
                               double annualRate,
                               bool   monthly,
                               const std::string& datetime)
{
    if (!node) return;

    applyInterest(node->left, annualRate, monthly, datetime);

    // amount of interest for this account
    double rateToApply = monthly ? (annualRate / 12.0) : annualRate;
    double interest    = node->balance * rateToApply;

    if (interest != 0.0) {
        node->balance += interest;
        appendTransaction(node->historyHead,
                          node->historyTail,
                          TransactionType::Interest,
                          interest,
                          monthly ? "Monthly interest" : "Yearly interest",
                          datetime);
    }

    applyInterest(node->right, annualRate, monthly, datetime);
}

// Compute and print basic statistics
void AccountBST::statistics() const
{
    if (!root) {
        std::cout << "No accounts to report statistics.\n";
        return;
    }

    int count = 0;
    double totalBalance = 0.0;
    const AccountNode* richest = nullptr;

    statistics(root, count, totalBalance, richest);

    std::cout << "==== Bank Statistics ====\n";
    std::cout << "Number of accounts : " << count << "\n";
    std::cout << "Total balance      : " << totalBalance << "\n";
    if (count > 0) {
        std::cout << "Average balance    : " << (totalBalance / count) << "\n";
    }
    if (richest) {
        std::cout << "Richest account    : #" << richest->accountNumber
                  << " (" << richest->holderName << "), balance = "
                  << richest->balance << "\n";
    }
    std::cout << "=========================\n";
}

void AccountBST::statistics(const AccountNode* node,
                            int& count,
                            double& totalBalance,
                            const AccountNode*& richest)
{
    if (!node) return;

    statistics(node->left, count, totalBalance, richest);

    ++count;
    totalBalance += node->balance;
    if (!richest || node->balance > richest->balance) {
        richest = node;
    }

    statistics(node->right, count, totalBalance, richest);
}
