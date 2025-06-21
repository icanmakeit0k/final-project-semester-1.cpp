#ifndef BOOK_H
#define BOOK_H

#include <string>

// This class is a simple data container. Its only job is to hold the
// information for a single book.
class Book
{
public:
    // --- Properties ---
    std::string isbn;
    int copyId; // +++ ADDED: To track individual copies of the same book.
    std::string title;
    std::string author;
    bool isCheckedOut = false;
    std::string borrowerUsername = "";
};

#endif // BOOK_H
