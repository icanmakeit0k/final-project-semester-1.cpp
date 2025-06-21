// NEW FILE
#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>

// A simple namespace for holding our ANSI color codes for a clean, modern theme.
namespace Color
{
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";

    // Regular Colors
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";

    // Bold Colors
    const std::string BOLD_RED = "\033[1;31m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_YELLOW = "\033[1;33m";
    const std::string BOLD_BLUE = "\033[1;34m";
    const std::string BOLD_MAGENTA = "\033[1;35m";
    const std::string BOLD_CYAN = "\033[1;36m";
    const std::string BOLD_WHITE = "\033[1;37m";
}

#endif // COLORS_HPP