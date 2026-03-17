// =============================================================================
// 01_hello_world.cpp — Your first C++ program
// =============================================================================
//
// COMPILE:  g++ -o hello 01_hello_world.cpp
// RUN:      ./hello
//
// C++ ANATOMY:
//   #include   → pulls in a library (like Python's import)
//   main()     → every C++ program starts here; returns int (0 = success)
//   std::cout  → standard character output (the "print" of C++)
//   <<         → stream insertion operator — sends data to cout
//   std::endl  → flushes buffer and adds newline  (or use "\n" — faster)
//   ;          → every statement ends with a semicolon
// =============================================================================

#include <iostream>   // gives us std::cout, std::cin, std::endl

int main()
{
    std::cout << "Hello, World!" << std::endl;

    // You can chain multiple << operators on one line
    std::cout << "Learning C++ on " << "Linux" << "\n";

    // Using "using namespace std" saves typing std:: everywhere
    // (common in small files / tutorials — but avoid in headers!)
    using namespace std;
    cout << "Same thing, shorter syntax." << endl;

    return 0;   // 0 tells the OS "program finished successfully"
}

// =============================================================================
// KEY TAKEAWAYS
//   - #include <iostream>  is mandatory for any I/O
//   - main() returns int  (the OS reads it)
//   - std:: prefix comes from the "std" (standard) namespace
//   - "\n" vs std::endl:  "\n" is faster (no buffer flush)
// =============================================================================
