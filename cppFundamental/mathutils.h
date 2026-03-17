// =============================================================================
// mathutils.h — Header file (declaration only, no implementation)
// =============================================================================
//
// HEADER FILES (.h / .hpp)
//   - Declare what exists (function signatures, class definitions, constants)
//   - Never define function bodies here (except inline / template functions)
//   - #pragma once OR include guards prevent double-inclusion
//
// .cpp FILES
//   - Define (implement) the bodies
//   - #include their own header
// =============================================================================

#pragma once            // Modern include guard — compiler-specific but universal
// --- OR use the classic guard ---
// #ifndef MATHUTILS_H
// #define MATHUTILS_H
// ... declarations ...
// #endif

#include <vector>
#include <string>

namespace MathUtils {   // Namespaces avoid name collisions between libraries

    // Function declarations (no body here)
    int    factorial(int n);
    double mean(const std::vector<double>& data);
    bool   isPrime(int n);

    // Inline function (body IS in the header — compiler may inline the call)
    inline int clamp(int val, int lo, int hi) {
        return (val < lo) ? lo : (val > hi) ? hi : val;
    }

    // Constant (constexpr in header is fine — no linker issues)
    constexpr double PI = 3.14159265358979323846;

}  // namespace MathUtils
