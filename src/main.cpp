#include "LibraryManager.h"
#include "User.h"
#include <iostream>
#include <string>
#include <limits>
#include "colors.hpp"

// --- Helper Functions for UI ---
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// --- Menus ---
void showLibrarianMenu()
{
    std::cout << "\n"
              << Color::BOLD_CYAN << "--- Librarian Menu ---\n"
              << Color::RESET
              << Color::CYAN << "--- Book Management ---\n"
              << Color::RESET
              << "1. Add New Book\n"
              << "2. Remove Book\n"
              << "3. Display All Books\n"
              << "4. Search for a Book\n"
              << "5. Check Out a Book\n"
              << "6. Return a Book\n"
              << Color::CYAN << "--- User Management ---\n"
              << Color::RESET
              << "7. Add New User\n"
              << "8. Remove User\n"
              << "10. Display All Users\n"
              << "11. Search for a User\n"
              << "-----------------------\n"
              << "9. Logout\n"
              << "-----------------------\n"
              << Color::BOLD_YELLOW << "Enter your choice: " << Color::RESET;
}

void showMemberMenu()
{
    std::cout << "\n"
              << Color::BOLD_CYAN << "--- Member Menu ---\n"
              << Color::RESET
              << "1. Display All Books\n"
              << "2. Search for a Book\n"
              << "3. Check Out a Book\n"
              << "4. Return a Book\n"
              << "9. Logout\n"
              << "---------------------\n"
              << Color::BOLD_YELLOW << "Enter your choice: " << Color::RESET;
}

// --- Main Application Logic ---
void librarianSession(LibraryManager &manager)
{
    int choice = 0;
    while (choice != 9)
    {
        clearScreen();
        showLibrarianMenu();
        std::cin >> choice;
        if (std::cin.fail())
        {
            std::cout << Color::BOLD_RED << "Invalid input. Please enter a number.\n"
                      << Color::RESET;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pauseScreen();
            continue;
        }
        switch (choice)
        {
        case 1:
            manager.addBook();
            pauseScreen();
            break;
        case 2:
            manager.removeBook();
            pauseScreen();
            break;
        case 3:
            manager.displayAllBooks();
            break;
        case 4:
            manager.searchBookByTitle();
            pauseScreen();
            break;
        case 5:
            manager.checkOutBook();
            pauseScreen();
            break;
        case 6:
            manager.returnBook();
            pauseScreen();
            break;
        case 7:
            manager.addUser();
            pauseScreen();
            break;
        case 8:
            manager.removeUser();
            pauseScreen();
            break;
        case 10:
            manager.displayAllUsers();
            break; // <-- The only change in this file
        case 11:
            manager.searchUserByUsername();
            pauseScreen();
            break;
        case 9:
            std::cout << Color::YELLOW << "Logging out...\n"
                      << Color::RESET;
            pauseScreen();
            break;
        default:
            std::cout << Color::BOLD_RED << "Invalid choice. Please try again.\n"
                      << Color::RESET;
            pauseScreen();
            break;
        }
    }
}

void memberSession(LibraryManager &manager)
{
    int choice = 0;
    while (choice != 9)
    {
        clearScreen();
        showMemberMenu();
        std::cin >> choice;
        if (std::cin.fail())
        {
            std::cout << Color::BOLD_RED << "Invalid input. Please enter a number.\n"
                      << Color::RESET;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pauseScreen();
            continue;
        }
        switch (choice)
        {
        case 1:
            manager.displayAllBooks();
            break;
        case 2:
            manager.searchBookByTitle();
            pauseScreen();
            break;
        case 3:
            manager.checkOutBook();
            pauseScreen();
            break;
        case 4:
            manager.returnBook();
            pauseScreen();
            break;
        case 9:
            std::cout << Color::YELLOW << "Logging out...\n"
                      << Color::RESET;
            pauseScreen();
            break;
        default:
            std::cout << Color::BOLD_RED << "Invalid choice. Please try again.\n"
                      << Color::RESET;
            pauseScreen();
            break;
        }
    }
}

int main()
{
    LibraryManager myLibrary("../data/books.csv", "../data/users.csv");

    while (true)
    {
        clearScreen();
        std::cout << Color::BOLD_BLUE << "=========================================\n";
        std::cout << Color::BOLD_CYAN << " Welcome to the Library Management System\n";
        std::cout << Color::BOLD_BLUE << "=========================================\n"
                  << Color::RESET;

        User *currentUser = nullptr;
        while (currentUser == nullptr)
        {
            std::string username, password;
            std::cout << "\n"
                      << Color::BOLD_WHITE << "--- Please Login ---\n"
                      << Color::RESET;
            std::cout << Color::YELLOW << "Enter username (or 'exit' to close): " << Color::RESET;
            std::cin >> username;

            if (username == "exit")
            {
                std::cout << "Thank you for using the system. Goodbye!\n";
                return 0;
            }

            std::cout << Color::YELLOW << "Enter password: " << Color::RESET;
            std::cin >> password;

            currentUser = myLibrary.validateUser(username, password);

            if (currentUser == nullptr)
            {
                std::cout << "\n"
                          << Color::BOLD_RED << "Login failed. Please check your credentials and try again.\n"
                          << Color::RESET;
            }
        }

        clearScreen();
        std::cout << Color::BOLD_GREEN << "Login successful! Welcome, " << currentUser->getUsername() << ".\n"
                  << Color::RESET;
        pauseScreen();

        if (currentUser->getRole() == UserRole::LIBRARIAN)
        {
            librarianSession(myLibrary);
        }
        else
        {
            memberSession(myLibrary);
        }
    }
    return 0;
}
