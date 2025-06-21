#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// We are putting a simplified User class directly in this file for the test.
class User
{
public:
    User(const std::string &username, const std::string &password)
    {
        m_username = username;
        m_password = password;
    }
    std::string getUsername() const { return m_username; }
    bool checkPassword(const std::string &attempt) const { return attempt == m_password; }

private:
    std::string m_username;
    std::string m_password;
};

// not important!

int main()
{
    // --- THIS IS THE MOST IMPORTANT PART ---
    // We are using the full, absolute path to your users file.
    // This removes any confusion about where the program is running from.
    std::string user_filepath = "/Users/louistom/Documents/LibraryManagementSystem/LibraryManagementSystem-G2/data/users.csv";

    std::cout << "--- Login Test Program ---\n";
    std::cout << "Attempting to open file at: " << user_filepath << "\n";

    std::vector<User> users_in_memory;
    std::ifstream inputFile(user_filepath);

    // Check if the file was successfully opened.
    if (!inputFile.is_open())
    {
        std::cerr << "CRITICAL ERROR: Failed to open the file. Please check the path and file permissions.\n";
        return 1; // Exit with an error code
    }

    std::cout << "SUCCESS: File was opened.\n";

    // Read the file line by line.
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        std::cout << "Read line from file: [" << line << "]\n";

        std::stringstream ss(line);
        std::string username, password, role_str;

        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, role_str); // Read the role just to consume it

        users_in_memory.emplace_back(username, password);
    }

    std::cout << "\nFinished reading file. Total users in memory: " << users_in_memory.size() << "\n\n";

    // --- Now, let's test the login check ---
    std::string login_attempt_user = "admin";
    std::string login_attempt_pass = "admin123";

    std::cout << "Attempting to log in as user: [" << login_attempt_user << "]\n";

    bool login_successful = false;
    for (const auto &user : users_in_memory)
    {
        std::cout << "Checking against stored user: [" << user.getUsername() << "]\n";
        if (user.getUsername() == login_attempt_user && user.checkPassword(login_attempt_pass))
        {
            login_successful = true;
            break; // Found a match, exit the loop <---- please be aware of this !
        }
    }

    std::cout << "\n--- FINAL RESULT ---\n";
    if (login_successful)
    {
        std::cout << "LOGIN SUCCEEDED! The problem is with your main project's setup.\n";
    }
    else
    {
        std::cout << "LOGIN FAILED! The problem is with the CSV file itself (e.g., extra spaces, invisible characters).\n"; // this code bugs takes me hours to fix it!
    }

    return 0;
}
