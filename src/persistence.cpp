#include "persistence.h"

#include "account_bst.h"
#include "transaction_list.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace bank {

/// Convert enum TransactionType to string for CSV.
static const char* transactionTypeToString(TransactionType type) {
    switch (type) {
        case TransactionType::Deposit:  return "Deposit";
        case TransactionType::Withdraw: return "Withdraw";
        case TransactionType::Interest: return "Interest";
        default:                        return "Unknown";
    }
}

/// Parse string into TransactionType.
/// Returns true on success, false if unknown.
static bool stringToTransactionType(const std::string& s,
                                    TransactionType& out) {
    if (s == "Deposit") {
        out = TransactionType::Deposit;
        return true;
    }
    if (s == "Withdraw" || s == "Withdrawal") {
        out = TransactionType::Withdraw;
        return true;
    }
    if (s == "Interest") {
        out = TransactionType::Interest;
        return true;
    }
    return false;
}

/// Helper: recursively save accounts + their transactions (in-order traversal).
static void saveAccountsInorder(const AccountNode* node,
                                std::ofstream& accountsOut,
                                std::ofstream& txOut) {
    if (!node) {
        return;
    }

    // 1) Left subtree
    saveAccountsInorder(node->left, accountsOut, txOut);

    // 2) This account
    const Account& acc = node->data;
    accountsOut << acc.accountNumber << ','
                << acc.holderName    << ','
                << acc.balance       << '\n';

    // 3) All transactions for this account
    const Transaction* current = acc.historyHead;
    while (current != nullptr) {
        txOut << acc.accountNumber << ','
              << transactionTypeToString(current->type) << ','
              << current->amount << ','
              << current->datetime << '\n';
        current = current->next;
    }

    // 4) Right subtree
    saveAccountsInorder(node->right, accountsOut, txOut);
}

bool saveBankToFiles(const Bank& bank,
                     const std::string& accountsFile,
                     const std::string& transactionsFile) {
    std::ofstream accOut(accountsFile);
    std::ofstream txOut(transactionsFile);

    if (!accOut.is_open() || !txOut.is_open()) {
        std::cerr << "Error: could not open output files for saving bank data.\n";
        return false;
    }

    // Optional: write headers so Excel sees column names.
    accOut << "accountNumber,holderName,balance\n";
    txOut << "accountNumber,type,amount,datetime\n";

    saveAccountsInorder(bank.accountsRoot, accOut, txOut);

    return true;
}

bool loadBankFromFiles(Bank& bank,
                       const std::string& accountsFile,
                       const std::string& transactionsFile) {
    bool anyLoaded = false;

    // ---- Load accounts ----
    {
        std::ifstream accIn(accountsFile);
        if (!accIn.is_open()) {
            std::cout << "No accounts file '" << accountsFile
                      << "' found. Starting with empty accounts.\n";
        } else {
            std::string line;

            // Skip header line (if present)
            std::getline(accIn, line);

            while (std::getline(accIn, line)) {
                if (line.empty()) {
                    continue;
                }

                std::stringstream ss(line);
                std::string accNumStr, name, balanceStr;

                if (!std::getline(ss, accNumStr, ',')) continue;
                if (!std::getline(ss, name, ','))       continue;
                if (!std::getline(ss, balanceStr, ',')) continue;

                try {
                    int accNum     = std::stoi(accNumStr);
                    double balance = std::stod(balanceStr);

                    // Use high-level API to create account.
                    // We trust the CSV not to contain duplicates.
                    createAccount(bank, accNum, name, balance);
                    anyLoaded = true;
                } catch (...) {
                    std::cerr << "Warning: invalid line in accounts file: "
                              << line << '\n';
                }
            }
        }
    }

    // ---- Load transactions ----
    {
        std::ifstream txIn(transactionsFile);
        if (!txIn.is_open()) {
            std::cout << "No transactions file '" << transactionsFile
                      << "' found. Starting with empty histories.\n";
        } else {
            std::string line;

            // Skip header
            std::getline(txIn, line);

            while (std::getline(txIn, line)) {
                if (line.empty()) {
                    continue;
                }

                std::stringstream ss(line);
                std::string accNumStr, typeStr, amountStr, datetime;

                if (!std::getline(ss, accNumStr, ',')) continue;
                if (!std::getline(ss, typeStr, ','))   continue;
                if (!std::getline(ss, amountStr, ',')) continue;
                if (!std::getline(ss, datetime))       continue; // remainder

                try {
                    int accNum    = std::stoi(accNumStr);
                    double amount = std::stod(amountStr);

                    TransactionType type;
                    if (!stringToTransactionType(typeStr, type)) {
                        std::cerr << "Warning: unknown transaction type '"
                                  << typeStr << "' in line: " << line << '\n';
                        continue;
                    }

                    AccountNode* node = searchAccount(bank.accountsRoot, accNum);
                    if (!node) {
                        std::cerr << "Warning: transaction for non-existing account #"
                                  << accNum << " in line: " << line << '\n';
                        continue;
                    }

                    // Append transaction to this account's history.
                    addTransaction(node->data.historyHead, type, amount, datetime);
                    anyLoaded = true;
                } catch (...) {
                    std::cerr << "Warning: invalid line in transactions file: "
                              << line << '\n';
                }
            }
        }
    }

    if (anyLoaded) {
        std::cout << "Loaded existing bank data from files.\n";
    } else {
        std::cout << "No existing bank data loaded.\n";
    }

    return anyLoaded;
}

} // namespace bank
