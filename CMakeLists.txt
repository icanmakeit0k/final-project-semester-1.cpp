# Specifies the minimum version of CMake required.
cmake_minimum_required(VERSION 3.14)

# Defines the name of your project.
project(LibraryManagementSystem)

# This section automatically downloads the 'tabulate' library.
include(FetchContent)
FetchContent_Declare(
    tabulate
    GIT_REPOSITORY https://github.com/p-ranav/tabulate
    GIT_TAG        v1.5
)
FetchContent_MakeAvailable(tabulate)

# Creates our final program, named "MyLibraryApp".
# MODIFIED: Removed Book.cpp as it is not needed.
add_executable(
    MyLibraryApp
    src/main.cpp
    src/User.cpp
    src/LibraryManager.cpp
)

# Tells the compiler to look inside the 'src' folder for header files (.h).
target_include_directories(MyLibraryApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Links our app with the 'tabulate' library.
target_link_libraries(MyLibraryApp PRIVATE tabulate)