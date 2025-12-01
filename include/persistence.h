#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include "bank_service.h"

namespace bank {

    /// Save all accounts and their transaction histories to two CSV files.
    /// Format:
    ///   accounts.csv:     accountNumber,holderName,balance
    ///   transactions.csv: accountNumber,type,amount,datetime
    ///
    /// Returns true on success, false on failure.
    bool saveBankToFiles(const Bank& bank,
                         const std::string& accountsFile,
                         const std::string& transactionsFile);

    /// Load accounts and histories from two CSV files into an existing Bank.
    ///
    /// If files do not exist, this function prints a message and returns false.
    /// If some data is loaded, returns true.
    bool loadBankFromFiles(Bank& bank,
                           const std::string& accountsFile,
                           const std::string& transactionsFile);

} // namespace bank

#endif // PERSISTENCE_H
