#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace bank {

    /// Returns current local date-time formatted as "YYYY-MM-DD HH:MM:SS".
    ///
    /// This is used when creating transactions so every operation has
    /// a human-readable timestamp.
    std::string getCurrentDateTime();

    /// Clears any leftover characters from the standard input buffer
    /// until a newline is found.
    ///
    /// Use this after failed std::cin >> operations to reset the
    /// input stream to a clean state.
    void clearInput();

    /// Prints a message and waits until the user presses ENTER.
    ///
    /// Useful to pause the console so users can read messages
    /// before the next screen/menu.
    void waitForEnter();

} // namespace bank

#endif // UTILS_H
