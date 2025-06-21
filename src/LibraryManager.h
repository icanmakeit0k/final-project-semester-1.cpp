#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include "Book.h"
#include "User.h"
#include <vector>
#include <string>

// This class handles all the backend logic.
class LibraryManager
{
public:
    // --- Constructor ---
    LibraryManager(const std::string &books_path, const std::string &users_path);

    // --- Public User Management Functions ---
    User *validateUser(const std::string &username, const std::string &password);
    void addUser();
    void removeUser();
    void displayAllUsers();
    void searchUserByUsername();

    // --- Public Book Management Functions ---
    void addBook();
    void displayAllBooks();
    void checkOutBook();
    void returnBook();
    void searchBookByTitle();
    void removeBook();

private:
    // --- Private Helper Functions (Internal use only) ---
    void loadBooks();
    void saveBooks();
    void loadUsers();
    void saveUsers();
    Book *findBookByISBN(const std::string &isbn);
    void displayPaginatedBooks(const std::vector<Book> &books);
    void displayPaginatedUsers(const std::vector<User> &users); // <-- Added this line

    // --- Private Properties ---
    std::string m_books_filepath;
    std::string m_users_filepath;
    std::vector<Book> m_books;
    std::vector<User> m_users;
};

#endif // LIBRARYMANAGER_H
