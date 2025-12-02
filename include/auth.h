#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <vector>

namespace bank {

    /// Simple representation of a system user (admin).
    struct User {
        std::string username;
        std::string password;
        bool isAdmin{false};
    };

    /// Initialize list of users. For now we hard-code a single admin.
    void initUsers(std::vector<User>& users);

    /// Check if given username + password matches any known user.
    bool checkCredentials(const std::vector<User>& users,
                          const std::string& username,
                          const std::string& password);

    /// Returns true if login successful within maxAttempts, false otherwise.
    bool login(const std::vector<User>& users, int maxAttempts = 3);

} // namespace bank

#endif // AUTH_H
