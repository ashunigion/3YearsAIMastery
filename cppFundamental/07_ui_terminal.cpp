// =============================================================================
// 07_ui_terminal.cpp — Terminal UI with formatted output, colour, menus
// =============================================================================
//
// COMPILE:  g++ -std=c++17 -o ui 07_ui_terminal.cpp
// RUN:      ./ui
//
// "UI" in pure C++ console programs uses:
//   - ANSI escape codes    → colour, bold, cursor movement
//   - iomanip / format     → table alignment, precision
//   - ncurses (optional)   → full-screen TUI (not used here — no extra dep)
// =============================================================================

#include <iostream>
#include <iomanip>      // setw, setfill, left, right, fixed, setprecision
#include <string>
#include <vector>
#include <limits>       // numeric_limits (for input clearing)
#include <sstream>

// =============================================================================
// ANSI ESCAPE CODE HELPERS
// (Work in any POSIX terminal — Linux, macOS)
// =============================================================================
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string CYAN    = "\033[36m";
    const std::string MAGENTA = "\033[35m";
    const std::string BG_BLUE = "\033[44m";
}

// =============================================================================
// UTILITY: Print a banner / section header
// =============================================================================
void printBanner(const std::string& title)
{
    const int WIDTH = 60;
    std::string bar(WIDTH, '=');
    std::cout << Color::BOLD << Color::CYAN
              << bar << "\n"
              << std::setw((WIDTH + static_cast<int>(title.size())) / 2)
              << title << "\n"
              << bar
              << Color::RESET << "\n";
}

// =============================================================================
// UTILITY: Pretty table
// =============================================================================
struct Row { std::string name; int age; double score; };

void printTable(const std::vector<Row>& rows)
{
    // Header
    std::cout << Color::BOLD
              << std::left  << std::setw(15) << "Name"
              << std::right << std::setw(6)  << "Age"
              << std::right << std::setw(10) << "Score"
              << Color::RESET << "\n";

    std::cout << std::string(31, '-') << "\n";

    // Rows
    for (const auto& r : rows) {
        std::cout << std::left  << std::setw(15) << r.name
                  << std::right << std::setw(6)  << r.age
                  << std::right << std::setw(10)
                  << std::fixed << std::setprecision(2) << r.score
                  << "\n";
    }
}

// =============================================================================
// UTILITY: Progress bar
// =============================================================================
void printProgress(int current, int total, int barWidth = 40)
{
    float pct = static_cast<float>(current) / total;
    int   filled = static_cast<int>(pct * barWidth);

    std::cout << "\r" << Color::GREEN << "[";
    for (int i = 0; i < barWidth; ++i)
        std::cout << (i < filled ? '#' : ' ');
    std::cout << "] "
              << std::setw(3) << static_cast<int>(pct * 100) << "%"
              << Color::RESET;
    std::cout.flush();
}

// =============================================================================
// UTILITY: Safe integer input
// =============================================================================
int getIntInput(const std::string& prompt, int lo, int hi)
{
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= lo && val <= hi)
            return val;
        std::cout << Color::RED << "  Invalid — enter a number between "
                  << lo << " and " << hi << Color::RESET << "\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// =============================================================================
// MENU DEMO
// =============================================================================
void runMenu()
{
    printBanner("MAIN MENU");
    std::cout << "  " << Color::YELLOW << "[1]" << Color::RESET << " View leaderboard\n";
    std::cout << "  " << Color::YELLOW << "[2]" << Color::RESET << " Show progress\n";
    std::cout << "  " << Color::YELLOW << "[3]" << Color::RESET << " Quit\n\n";

    int choice = getIntInput("Your choice: ", 1, 3);

    switch (choice) {
        case 1: {
            printBanner("LEADERBOARD");
            std::vector<Row> rows = {
                {"Alice",   28, 98.50},
                {"Bob",     34, 87.25},
                {"Charlie", 22, 93.00},
                {"Diana",   31, 75.80},
            };
            printTable(rows);
            break;
        }
        case 2: {
            printBanner("PROGRESS");
            int total = 50;
            for (int i = 0; i <= total; ++i) {
                printProgress(i, total);
                // Simulate work (busy loop — replace with real work)
                for (volatile int j = 0; j < 2000000; ++j) {}
            }
            std::cout << "\nComplete!\n";
            break;
        }
        case 3:
            std::cout << Color::GREEN << "Goodbye!\n" << Color::RESET;
            break;
    }
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    // ── Formatted numbers ─────────────────────────────────────────────────────
    printBanner("NUMBER FORMATTING");

    double pi = 3.14159265358979;
    std::cout << "Default:     " << pi                                     << "\n";
    std::cout << "fixed 2dp:   " << std::fixed << std::setprecision(2) << pi << "\n";
    std::cout << "scientific:  " << std::scientific << pi                  << "\n";
    std::cout << "hex int:     " << std::hex << 255                        << "\n";
    std::cout << "octal:       " << std::oct << 255                        << "\n";
    std::cout << std::dec;  // reset to decimal

    // ── Zero-padded numbers ───────────────────────────────────────────────────
    std::cout << "Padded:      " << std::setfill('0') << std::setw(6) << 42 << "\n";
    std::cout << std::setfill(' ');  // reset fill char

    // ── Menu ──────────────────────────────────────────────────────────────────
    runMenu();

    return 0;
}

// =============================================================================
// KEY TAKEAWAYS
//   - ANSI codes: "\033[31m" = red, "\033[0m" = reset  (works on Linux/macOS)
//   - iomanip:  setw(n) pads to n chars | setprecision(n) | fixed | scientific
//   - Always reset stream state after using hex/oct/fixed → << std::dec / defaultfloat
//   - std::cin.clear() + ignore() clears bad input state after non-integer input
//   - For full-screen TUI → use ncurses or FTXUI library
// =============================================================================
