#include "utils.h"

#include <chrono>    // std::chrono::system_clock
#include <ctime>     // std::time_t, std::localtime
#include <iomanip>   // std::put_time
#include <iostream>  // std::cout, std::cin
#include <limits>    // std::numeric_limits
#include <sstream>   // std::ostringstream

namespace bank {

    std::string getCurrentDateTime() {
        // 1) Get current time point from the system clock.
        auto now = std::chrono::system_clock::now();

        // 2) Convert to time_t (seconds since epoch).
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // 3) Convert to local calendar time (year, month, day, etc.).
        // std::localtime returns a pointer to a static std::tm.
        std::tm* tm_ptr = std::localtime(&now_c);
        if (!tm_ptr) {
            // If conversion fails, return a fallback string.
            return "0000-00-00 00:00:00";
        }

        // 4) Format the time into a string using std::put_time.
        std::ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void clearInput() {
        // Reset error flags on the stream, in case a previous read failed.
        std::cin.clear();

        // Ignore characters until newline or EOF.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void waitForEnter() {
        std::cout << "Press ENTER to continue...";
        // Make sure there is no leftover data in input buffer.
        clearInput();
        // Wait for a newline (user pressing ENTER).
        std::cin.get();
    }

} // namespace bank
