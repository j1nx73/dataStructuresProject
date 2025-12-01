#include "auth.h"

#include <iostream>

namespace bank {

    void initUsers(std::vector<User>& users) {
        users.clear();

        // For this project we hard-code one admin account.
        // You can later extend this to load from file.
        users.push_back(User{"admin", "1234", true});
    }

    bool checkCredentials(const std::vector<User>& users,
                          const std::string& username,
                          const std::string& password) {
        for (const auto& u : users) {
            if (u.username == username && u.password == password) {
                return true;
            }
        }
        return false;
    }

    bool login(const std::vector<User>& users, int maxAttempts) {
        std::cout << "=====================================\n";
        std::cout << "        Banking System Login         \n";
        std::cout << "=====================================\n";

        // We use std::getline here (no UI helpers) so auth module is independent.
        for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
            std::string username;
            std::string password;

            std::cout << "Username: ";
            std::getline(std::cin, username);

            std::cout << "Password: ";
            std::getline(std::cin, password);

            if (checkCredentials(users, username, password)) {
                std::cout << "Login successful. Welcome, " << username << "!\n";
                return true;
            }

            std::cout << "Invalid username or password (attempt "
                      << attempt << " of " << maxAttempts << ").\n";
        }

        std::cout << "Too many failed login attempts. Exiting.\n";
        return false;
    }

} // namespace bank
