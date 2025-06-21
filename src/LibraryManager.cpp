#include "LibraryManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath>
#include "tabulate/table.hpp"
#include "colors.hpp"

// Constructor: Loads all data when the program starts.
LibraryManager::LibraryManager(const std::string &books_path, const std::string &users_path)
{
    m_books_filepath = books_path;
    m_users_filepath = users_path;
    loadBooks();
    loadUsers();
}

// --- File I/O ---

void LibraryManager::loadBooks()
{
    std::ifstream inputFile(m_books_filepath);
    if (!inputFile.is_open())
    {
        std::cerr << Color::BOLD_RED << "ERROR: Could not open books data file: " << m_books_filepath << Color::RESET << std::endl;
        return;
    }
    m_books.clear();
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
            continue;
        if (line.back() == '\r')
            line.pop_back();

        std::stringstream ss(line);
        std::string isbn, title, author, status_str, borrower;
        std::getline(ss, isbn, ',');
        std::getline(ss, title, ',');
        std::getline(ss, author, ',');
        std::getline(ss, status_str, ',');
        std::getline(ss, borrower);

        Book newBook;
        newBook.isbn = isbn;
        newBook.title = title;
        newBook.author = author;
        newBook.isCheckedOut = (status_str == "1");
        newBook.borrowerUsername = borrower;
        m_books.push_back(newBook);
    }
}

void LibraryManager::saveBooks()
{
    std::ofstream outputFile(m_books_filepath);
    if (!outputFile.is_open())
    {
        std::cerr << Color::BOLD_RED << "ERROR: Could not write to books data file: " << m_books_filepath << Color::RESET << std::endl;
        return;
    }
    for (const auto &book : m_books)
    {
        outputFile << book.isbn << "," << book.title << "," << book.author
                   << "," << (book.isCheckedOut ? "1" : "0") << "," << book.borrowerUsername << std::endl;
    }
}

void LibraryManager::loadUsers()
{
    std::ifstream inputFile(m_users_filepath);
    if (!inputFile.is_open())
    {
        std::cerr << Color::BOLD_RED << "ERROR: Could not open users data file: " << m_users_filepath << Color::RESET << std::endl;
        return;
    }
    m_users.clear();
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
            continue;
        if (line.back() == '\r')
            line.pop_back();
        std::stringstream ss(line);
        std::string username, password, role_str;
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, role_str);
        UserRole role = (role_str == "0" ? UserRole::LIBRARIAN : UserRole::MEMBER);
        m_users.emplace_back(username, password, role);
    }
}

void LibraryManager::saveUsers()
{
    std::ofstream outputFile(m_users_filepath);
    if (!outputFile.is_open())
    {
        std::cerr << Color::BOLD_RED << "ERROR: Could not write to users data file: " << m_users_filepath << Color::RESET << std::endl;
        return;
    }
    for (const auto &user : m_users)
    {
        int role_int = (user.getRole() == UserRole::LIBRARIAN) ? 0 : 1;
        outputFile << user.getUsername() << "," << user.getPassword() << "," << role_int << std::endl;
    }
}

// --- User Management ---

User *LibraryManager::validateUser(const std::string &username, const std::string &password)
{
    for (auto &user : m_users)
    {
        if (user.getUsername() == username && user.checkPassword(password))
            return &user;
    }
    return nullptr;
}

// === FUNCTION UPDATED with username and password validation ===
void LibraryManager::addUser()
{
    std::string new_username, new_password;
    int role_choice = -1;
    std::cout << "\n"
              << Color::BOLD_CYAN << "--- Add New User ---\n"
              << Color::RESET;

    // --- Username Input and Validation ---
    while (true)
    {
        std::cout << "  Enter new username: ";
        std::cin >> new_username;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        bool username_exists = false;
        for (const auto &user : m_users)
        {
            if (user.getUsername() == new_username)
            {
                username_exists = true;
                break;
            }
        }

        if (username_exists)
        {
            std::cout << Color::BOLD_RED << "Error: A user with the username '" << new_username << "' already exists. Please try another.\n"
                      << Color::RESET;
        }
        else
        {
            break; // Username is unique, exit loop
        }
    }

    // --- Password Input and Validation ---
    while (true)
    {
        std::cout << "  Enter new password (numeric only): ";
        std::cin >> new_password;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        bool is_numeric = true;
        if (new_password.empty())
        {
            is_numeric = false;
        }
        else
        {
            for (char const &c : new_password)
            {
                if (std::isdigit(c) == 0)
                {
                    is_numeric = false;
                    break;
                }
            }
        }

        if (!is_numeric)
        {
            std::cout << Color::BOLD_RED << "Error: Password must contain only numbers. Please try again.\n"
                      << Color::RESET;
        }
        else
        {
            break; // Password is valid, exit loop
        }
    }

    // --- Role Selection ---
    while (role_choice != 0 && role_choice != 1)
    {
        std::cout << "  Enter Role (" << Color::BOLD_MAGENTA << "0" << Color::RESET << " for Librarian, "
                  << Color::BOLD_MAGENTA << "1" << Color::RESET << " for Member): ";
        std::cin >> role_choice;
        if (std::cin.fail() || (role_choice != 0 && role_choice != 1))
        {
            std::cout << Color::BOLD_RED << "Invalid input. Please enter 0 or 1.\n"
                      << Color::RESET;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            role_choice = -1;
        }
    }
    UserRole new_role = (role_choice == 0) ? UserRole::LIBRARIAN : UserRole::MEMBER;
    m_users.emplace_back(new_username, new_password, new_role);
    saveUsers();
    std::cout << "\n"
              << Color::BOLD_GREEN << "User '" << new_username << "' was added successfully!\n"
              << Color::RESET;
}

void LibraryManager::removeUser()
{
    std::string usernameToRemove;
    std::cout << "\nEnter username of the user to remove: ";
    std::cin >> usernameToRemove;
    if (usernameToRemove == "admin")
    {
        std::cout << Color::BOLD_RED << "Error: The default admin user cannot be removed." << Color::RESET << std::endl;
        return;
    }
    auto original_size = m_users.size();
    m_users.erase(
        std::remove_if(m_users.begin(), m_users.end(), [&](const User &user)
                       { return user.getUsername() == usernameToRemove; }),
        m_users.end());
    if (m_users.size() < original_size)
    {
        saveUsers();
        std::cout << Color::BOLD_GREEN << "User removed successfully." << Color::RESET << std::endl;
    }
    else
    {
        std::cout << Color::BOLD_RED << "Error: User not found." << Color::RESET << std::endl;
    }
}

void LibraryManager::displayAllUsers()
{
    if (m_users.empty())
    {
        std::cout << "\nThere are no users in the system." << std::endl;
        return;
    }
    std::sort(m_users.begin(), m_users.end(), [](const User &a, const User &b)
              { return a.getUsername() < b.getUsername(); });
    displayPaginatedUsers(m_users);
}

void LibraryManager::searchUserByUsername()
{
    std::string searchTerm;
    std::cout << "\nEnter username to search for: ";
    std::cin >> searchTerm;
    std::vector<User> foundUsers;
    for (const auto &user : m_users)
    {
        if (user.getUsername().find(searchTerm) != std::string::npos)
        {
            foundUsers.push_back(user);
        }
    }
    if (foundUsers.empty())
    {
        std::cout << "No users found matching that name." << std::endl;
    }
    else
    {
        std::cout << "\n--- User Search Results ---\n";
        tabulate::Table table;
        table.add_row({"Username", "Role"});
        for (const auto &user : foundUsers)
        {
            table.add_row({user.getUsername(), (user.getRole() == UserRole::LIBRARIAN ? "Librarian" : "Member")});
        }
        std::cout << table << std::endl;
    }
}

void LibraryManager::displayPaginatedUsers(const std::vector<User> &users)
{
    const int page_size = 5;
    int current_page = 1;
    const int total_records = users.size();
    const int total_pages = static_cast<int>(std::ceil(static_cast<double>(total_records) / page_size));
    char choice;

    do
    {
        system("clear");
        std::cout << "\n"
                  << Color::BOLD_CYAN << "--- All System Users (Sorted by Username) ---" << Color::RESET << std::endl;

        tabulate::Table table;
        table.add_row({"Username", "Role"});

        int start_index = (current_page - 1) * page_size;
        int end_index = std::min(start_index + page_size, total_records);

        for (int i = start_index; i < end_index; ++i)
        {
            const auto &user = users[i];
            bool is_librarian = (user.getRole() == UserRole::LIBRARIAN);
            std::string role_text = is_librarian ? "Librarian" : "Member";

            table.add_row({user.getUsername(), role_text});

            auto &row = table.row(table.size() - 1);
            if (is_librarian)
            {
                row[1].format().font_color(tabulate::Color::magenta).font_style({tabulate::FontStyle::bold});
            }
            else
            {
                row[1].format().font_color(tabulate::Color::none);
            }
        }

        table.format().border_top("=").border_bottom("=").border_left("|").border_right("|");
        table.format().corner("o").corner_color(tabulate::Color::blue).border_color(tabulate::Color::blue);
        table[0].format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::cyan).padding_top(1).padding_bottom(1);

        std::cout << table << std::endl;

        std::cout << Color::BOLD_WHITE << "Page " << current_page << " of " << total_pages << Color::RESET << std::endl;
        std::cout << Color::BOLD_YELLOW << "[N]" << Color::RESET << "ext Page | "
                  << Color::BOLD_YELLOW << "[P]" << Color::RESET << "revious Page | "
                  << Color::BOLD_YELLOW << "[Q]" << Color::RESET << "uit to Menu" << std::endl;
        std::cout << "Enter your choice: ";

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        choice = std::tolower(choice);

        if (choice == 'n' && current_page < total_pages)
            current_page++;
        if (choice == 'p' && current_page > 1)
            current_page--;

    } while (choice != 'q');
}

// --- Book Management Functions ---

void LibraryManager::displayAllBooks()
{
    if (m_books.empty())
    {
        std::cout << "\nThe library has no books." << std::endl;
        return;
    }
    std::sort(m_books.begin(), m_books.end(), [](const Book &a, const Book &b)
              { return a.title < b.title; });
    displayPaginatedBooks(m_books);
}

void LibraryManager::displayPaginatedBooks(const std::vector<Book> &books)
{
    const int page_size = 5;
    int current_page = 1;
    const int total_records = books.size();
    const int total_pages = static_cast<int>(std::ceil(static_cast<double>(total_records) / page_size));
    char choice;

    do
    {
        system("clear");
        std::cout << "\n"
                  << Color::BOLD_CYAN << "--- All Books in Library (Sorted by Title) ---" << Color::RESET << std::endl;

        tabulate::Table table;
        table.add_row({"ISBN", "Title", "Author", "Status"});

        int start_index = (current_page - 1) * page_size;
        int end_index = std::min(start_index + page_size, total_records);

        for (int i = start_index; i < end_index; ++i)
        {
            const auto &book = books[i];
            std::string status_text;

            if (book.isCheckedOut)
            {
                status_text = "Checked Out by: " + book.borrowerUsername;
            }
            else
            {
                status_text = "Available";
            }

            table.add_row({book.isbn, book.title, book.author, status_text});

            auto &row = table.row(table.size() - 1);
            if (!book.isCheckedOut)
            {
                row[3].format().font_color(tabulate::Color::green).font_style({tabulate::FontStyle::bold});
            }
            else
            {
                row.format().font_color(tabulate::Color::red);
            }
        }

        table.format().border_top("=").border_bottom("=").border_left("|").border_right("|");
        table.format().corner("o").corner_color(tabulate::Color::blue).border_color(tabulate::Color::blue);
        table[0].format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::cyan).padding_top(1).padding_bottom(1);

        std::cout << table << std::endl;

        std::cout << Color::BOLD_WHITE << "Page " << current_page << " of " << total_pages << Color::RESET << std::endl;
        std::cout << Color::BOLD_YELLOW << "[N]" << Color::RESET << "ext Page | "
                  << Color::BOLD_YELLOW << "[P]" << Color::RESET << "revious Page | "
                  << Color::BOLD_YELLOW << "[Q]" << Color::RESET << "uit to Menu" << std::endl;
        std::cout << "Enter your choice: ";

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        choice = std::tolower(choice);

        if (choice == 'n' && current_page < total_pages)
            current_page++;
        if (choice == 'p' && current_page > 1)
            current_page--;

    } while (choice != 'q');
}

void LibraryManager::checkOutBook()
{
    std::string isbn;
    std::cout << "\nEnter ISBN of the book to borrow: ";
    std::cin >> isbn;

    Book *book = findBookByISBN(isbn);

    if (book == nullptr)
    {
        std::cout << Color::BOLD_RED << "Error: Book not found." << Color::RESET << std::endl;
        return;
    }

    if (book->isCheckedOut)
    {
        std::cout << Color::YELLOW << "Sorry, this book is already checked out by user '" << book->borrowerUsername << "'." << Color::RESET << std::endl;
        return;
    }

    User *user = nullptr;
    while (true)
    { // Loop for authentication
        std::string username, password;
        std::cout << "\n"
                  << Color::BOLD_WHITE << "--- Please Verify Your Identity to Borrow ---\n"
                  << Color::RESET;
        std::cout << Color::YELLOW << "Enter your username: " << Color::RESET;
        std::cin >> username;
        std::cout << Color::YELLOW << "Enter your password: " << Color::RESET;
        std::cin >> password;

        user = validateUser(username, password);

        if (user != nullptr)
        {
            // Success! Break the loop.
            break;
        }
        else
        {
            // Failure
            std::cout << "\n"
                      << Color::BOLD_RED << "Authentication failed. Invalid username or password." << Color::RESET << std::endl;
            char choice;
            std::cout << "Would you like to (R)etry or (E)xit? ";
            std::cin >> choice;
            if (std::tolower(choice) == 'e')
            {
                std::cout << "Checkout cancelled." << std::endl;
                return; // Exit the function entirely
            }
            // If they choose 'r' or anything else, the loop will just continue
        }
    }

    // This part now only runs after a successful login
    book->isCheckedOut = true;
    book->borrowerUsername = user->getUsername();
    saveBooks();
    std::cout << "\n"
              << Color::BOLD_GREEN << "Successfully borrowed '" << book->title << "'!" << Color::RESET << std::endl;
}

void LibraryManager::returnBook()
{
    std::string isbn;
    std::cout << "\nEnter ISBN of the book to return: ";
    std::cin >> isbn;

    Book *book = findBookByISBN(isbn);

    if (book == nullptr)
    {
        std::cout << Color::BOLD_RED << "Error: Book not found." << Color::RESET << std::endl;
        return;
    }

    if (!book->isCheckedOut)
    {
        std::cout << Color::YELLOW << "This book is already in the library and was not checked out." << Color::RESET << std::endl;
        return;
    }

    std::string borrower = book->borrowerUsername;
    book->isCheckedOut = false;
    book->borrowerUsername = "";
    saveBooks();

    std::cout << "\n"
              << Color::BOLD_GREEN << "Successfully returned '" << book->title << "' (was borrowed by " << borrower << ")." << Color::RESET << std::endl;
}

void LibraryManager::addBook()
{
    Book newBook;
    std::string isbn_input;
    std::cout << "\n"
              << Color::BOLD_CYAN << "--- Add New Book ---\n"
              << Color::RESET;

    // --- ISBN Input and Validation ---
    while (true)
    {
        std::cout << "  Enter ISBN (numeric only): ";
        std::cin >> isbn_input;
        if (std::cin.fail())
        {
            std::cout << Color::BOLD_RED << "Invalid input. Please enter numbers only." << Color::RESET << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        bool is_numeric = true;
        for (char const &c : isbn_input)
        {
            if (std::isdigit(c) == 0)
            {
                is_numeric = false;
                break;
            }
        }

        if (!is_numeric)
        {
            std::cout << Color::BOLD_RED << "Error: ISBN must contain only numbers. Please try again." << Color::RESET << std::endl;
            continue;
        }

        if (findBookByISBN(isbn_input) != nullptr)
        {
            std::cout << Color::BOLD_RED << "\nError: A book with ISBN '" << isbn_input << "' already exists." << Color::RESET << std::endl;
            return;
        }

        newBook.isbn = isbn_input;
        break;
    }

    // --- Title and Author Input ---
    std::cout << "  Title: ";
    std::getline(std::cin, newBook.title);

    std::cout << "  Author: ";
    std::getline(std::cin, newBook.author);

    for (const auto &book : m_books)
    {
        if (book.title == newBook.title && book.author == newBook.author)
        {
            std::cout << Color::BOLD_RED << "\nError: A book with the same title and author already exists." << Color::RESET << std::endl;
            return;
        }
    }

    m_books.push_back(newBook);
    saveBooks();
    std::cout << "\n"
              << Color::BOLD_GREEN << "Book added successfully!\n"
              << Color::RESET;
}

void LibraryManager::removeBook()
{
    std::string isbn;
    std::cout << "\nEnter ISBN of the book to remove: ";
    std::cin >> isbn;
    auto original_size = m_books.size();
    m_books.erase(
        std::remove_if(m_books.begin(), m_books.end(), [&](const Book &book)
                       { return book.isbn == isbn; }),
        m_books.end());
    if (m_books.size() < original_size)
    {
        saveBooks();
        std::cout << "Book removed successfully." << std::endl;
    }
    else
    {
        std::cout << "Error: Book not found." << std::endl;
    }
}

void LibraryManager::searchBookByTitle()
{
    std::string searchTerm;
    std::cout << "\nEnter title to search for: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, searchTerm);
    std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
    std::vector<Book> foundBooks;
    for (const auto &book : m_books)
    {
        std::string bookTitle = book.title;
        std::transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);
        if (bookTitle.find(searchTerm) != std::string::npos)
        {
            foundBooks.push_back(book);
        }
    }
    if (foundBooks.empty())
    {
        std::cout << "No books found matching your search." << std::endl;
    }
    else
    {
        std::cout << "\n--- Search Results ---\n";
        tabulate::Table table;
        table.add_row({"ISBN", "Title", "Author", "Status"});
        for (const auto &book : foundBooks)
        {
            table.add_row({book.isbn, book.title, book.author, (book.isCheckedOut ? "Checked Out by: " + book.borrowerUsername : "Available")});
        }
        std::cout << table << std::endl;
    }
}

Book *LibraryManager::findBookByISBN(const std::string &isbn)
{
    for (auto &book : m_books)
    {
        if (book.isbn == isbn)
        {
            return &book;
        }
    }
    return nullptr;
}
