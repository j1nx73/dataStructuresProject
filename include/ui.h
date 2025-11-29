#ifndef UI_H
#define UI_H

#include "bank_service.h"

namespace bank {

    /// Runs the main terminal menu loop for the banking system.
    ///
    /// The function keeps asking the user for options until they choose to exit.
    /// It uses functions from bank_service.h to perform operations.
    void runMainMenu(Bank& bank);

} // namespace bank

#endif // UI_H
