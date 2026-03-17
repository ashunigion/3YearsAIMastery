// =============================================================================
// 02_variables.cpp — Types, literals, const, auto, casting, scope
// =============================================================================
//
// COMPILE:  g++ -std=c++17 -o vars 02_variables.cpp
// RUN:      ./vars
//
// C++ is STATICALLY TYPED — every variable has a fixed type at compile time.
// The compiler catches type mismatches before your program runs (unlike Python).
// =============================================================================

#include <iostream>
#include <string>    // std::string lives here
#include <limits>    // std::numeric_limits — shows min/max of each type

int main()
{
    // ── PRIMITIVE TYPES ──────────────────────────────────────────────────────
    //   Type         Typical size   Range (approx)
    //   bool         1 byte         true / false
    //   char         1 byte         -128 … 127  OR a single character
    //   int          4 bytes        -2 billion … +2 billion
    //   long long    8 bytes        ±9.2 × 10^18
    //   float        4 bytes        6-7 significant decimal digits
    //   double       8 bytes        15-16 significant decimal digits

    bool   flag   = true;
    char   letter = 'A';          // single quotes for char
    int    score  = 42;
    long long bigNum = 9'000'000'000LL;   // digit separators (C++14)
    float  pi_f   = 3.14159f;     // 'f' suffix → float  (no 'f' → double)
    double pi_d   = 3.14159265358979;

    std::cout << "bool:      " << flag   << "\n";   // prints 1 (true)
    std::cout << "char:      " << letter << "\n";
    std::cout << "int:       " << score  << "\n";
    std::cout << "long long: " << bigNum << "\n";
    std::cout << "float:     " << pi_f   << "\n";
    std::cout << "double:    " << pi_d   << "\n";

    // ── STRINGS ──────────────────────────────────────────────────────────────
    // std::string is NOT a primitive — it lives in <string>
    std::string name  = "Alice";
    std::string greet = "Hello, " + name + "!";  // concatenation with +
    std::cout << greet << "  length=" << greet.size() << "\n";

    // ── CONST & CONSTEXPR ────────────────────────────────────────────────────
    const int MAX_USERS = 100;      // value fixed at runtime
    // MAX_USERS = 200;             // ERROR: cannot modify a const

    constexpr double TAX_RATE = 0.15;  // evaluated at COMPILE TIME — faster
    std::cout << "Tax: " << TAX_RATE << "\n";

    // ── AUTO — let the compiler deduce the type ───────────────────────────────
    auto x     = 10;      // int
    auto y     = 3.14;    // double
    auto label = std::string("hello");   // std::string

    // Use auto freely; it makes code shorter without losing type safety.
    std::cout << "auto x=" << x << "  y=" << y << "\n";

    // ── TYPE CASTING ─────────────────────────────────────────────────────────
    int   a = 7, b = 2;
    double ratio_wrong = a / b;          // integer division → 3 (truncated!)
    double ratio_right = (double)a / b;  // C-style cast: promote a to double → 3.5
    double ratio_cpp   = static_cast<double>(a) / b;  // C++ preferred cast

    std::cout << "int division:    " << ratio_wrong << "\n";  // 3
    std::cout << "double division: " << ratio_right << "\n";  // 3.5
    std::cout << "static_cast:     " << ratio_cpp   << "\n";  // 3.5

    // ── SCOPE ────────────────────────────────────────────────────────────────
    // A variable only lives inside the { } block where it is declared.
    {
        int inner = 99;
        std::cout << "inner (visible here): " << inner << "\n";
    }
    // std::cout << inner;   // ERROR: 'inner' is out of scope here

    // ── INPUT FROM KEYBOARD ──────────────────────────────────────────────────
    // std::cin >> variable;   reads one whitespace-delimited token
    // std::getline(std::cin, str);  reads a whole line

    // (We skip live input here to keep the example runnable without user input)

    // ── NUMERIC LIMITS ───────────────────────────────────────────────────────
    std::cout << "int max: " << std::numeric_limits<int>::max() << "\n";

    return 0;
}

// =============================================================================
// KEY TAKEAWAYS
//   - Declare type before variable name:  int x = 5;
//   - const  → cannot change after init; constexpr → known at compile time
//   - auto   → compiler deduces type; use it — it's idiomatic modern C++
//   - Integer / integer = integer!  Cast at least one operand for real division
//   - static_cast<T>(x) is the C++ way to cast (safer than C-style (T)x)
//   - Variables die at the closing } of their block (RAII — more on this later)
// =============================================================================
