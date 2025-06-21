#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole
{
    LIBRARIAN,
    MEMBER
};

class User
{
public:
    User(const std::string &username, const std::string &password, UserRole role);

    std::string getUsername() const;
    std::string getPassword() const; // <-- This is the new function
    UserRole getRole() const;
    bool checkPassword(const std::string &password_attempt) const;

private:
    std::string m_username;
    std::string m_password;
    UserRole m_role;
};

#endif // USER_H
