// =============================================================================
// 05_imports_headers.cpp — #include, namespaces, header/implementation split
// =============================================================================
//
// COMPILE (two translation units linked together):
//   g++ -std=c++17 -o imports 05_imports_headers.cpp mathutils.cpp
// RUN:
//   ./imports
//
// WHY TWO FILES?
//   - mathutils.h   → the CONTRACT  (what you can call)
//   - mathutils.cpp → the CODE      (how it works)
//   - The linker stitches them together at build time.
//   - Other files include the .h — they never need to see the .cpp.
//   - This enables: separate compilation, build caching, and library hiding.
// =============================================================================

// ── Standard library headers (angle brackets = system search path) ────────────
#include <iostream>
#include <vector>
#include <string>
#include <fstream>      // file I/O
#include <sstream>      // string streams
#include <algorithm>
#include <cassert>      // assert()

// ── Your own headers (quotes = local directory first) ─────────────────────────
#include "mathutils.h"

// =============================================================================
// NAMESPACES
// =============================================================================
// std::    → Standard Library
// MathUtils:: → our custom namespace (defined in mathutils.h)
//
// "using namespace std;" saves typing but can cause name clashes in large code.
// Best practice: use it only in .cpp files, NEVER in header files.

using namespace std;    // OK here — small demo file

int main()
{
    // ── Using our custom library ──────────────────────────────────────────────
    cout << "5! = " << MathUtils::factorial(5) << "\n";      // 120

    vector<double> data = {2.0, 4.0, 6.0, 8.0, 10.0};
    cout << "mean = " << MathUtils::mean(data) << "\n";      // 6.0

    cout << "7 prime? " << boolalpha << MathUtils::isPrime(7) << "\n";   // true
    cout << "9 prime? " << boolalpha << MathUtils::isPrime(9) << "\n";   // false

    cout << "clamp(150,0,100) = " << MathUtils::clamp(150, 0, 100) << "\n"; // 100
    cout << "PI = " << MathUtils::PI << "\n";

    // ── Namespace alias ───────────────────────────────────────────────────────
    namespace MU = MathUtils;   // shorter alias
    cout << "6! = " << MU::factorial(6) << "\n";

    // ── File I/O ──────────────────────────────────────────────────────────────
    // Write a file
    {
        ofstream out("demo_output.txt");
        if (!out) { cerr << "Cannot open file\n"; return 1; }
        out << "Hello from C++\n";
        out << "Factorial of 7 = " << MU::factorial(7) << "\n";
    }  // ofstream closes automatically here (RAII)

    // Read it back
    {
        ifstream in("demo_output.txt");
        string line;
        while (getline(in, line))
            cout << "[file] " << line << "\n";
    }

    // ── String stream (build a string like a stream) ──────────────────────────
    ostringstream oss;
    oss << "Result: " << 3.14 << " finished";
    string result = oss.str();
    cout << result << "\n";

    // ── assert — debug-time check (disabled in release builds with -DNDEBUG) ──
    assert(MU::factorial(0) == 1);
    assert(MU::mean({1.0, 2.0, 3.0}) == 2.0);

    cout << "All assertions passed.\n";
    return 0;
}

// =============================================================================
// KEY TAKEAWAYS
//   - #include "myfile.h" → local header   |   #include <system.h> → system
//   - Separate .h (declaration) from .cpp (definition) → compile separately
//   - namespace Foo { } → groups names, avoids collisions
//   - RAII: ofstream, ifstream close themselves when they go out of scope
//   - assert() is for invariants you KNOW must be true — not user input checking
// =============================================================================
