#include "User.h"

User::User(const std::string &username, const std::string &password, UserRole role)
{
    m_username = username;
    m_password = password;
    m_role = role;
}

std::string User::getUsername() const
{
    return m_username;
}

// --- This is the new function's implementation ---
// This allows our LibraryManager to get the password for saving the file. //    <===== very crucial guys
std::string User::getPassword() const
{
    return m_password;
}
// ---------------------------------------------

UserRole User::getRole() const
{
    return m_role;
}

bool User::checkPassword(const std::string &password_attempt) const
{
    return password_attempt == m_password;
}
