// =============================================================================
// 09_documentation_style.cpp — Doxygen comments, code style, best practices
// =============================================================================
//
// DOXYGEN: industry-standard doc generator for C/C++/Java/Python
//   Install:  sudo apt install doxygen graphviz
//   Generate: doxygen -g Doxyfile    ← creates config
//             doxygen Doxyfile       ← generates HTML/PDF docs
//
// COMPILE:  g++ -std=c++17 -o docs 09_documentation_style.cpp
// =============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

// =============================================================================
// FILE-LEVEL DOC BLOCK
/// @file  09_documentation_style.cpp
/// @brief Demonstrates Doxygen documentation conventions in C++.
/// @author Your Name
/// @date   2026-03-17
/// @version 1.0
// =============================================================================

/// @brief Supported distance metrics.
enum class DistanceMetric {
    EUCLIDEAN,   ///< Straight-line distance √(Σ(aᵢ-bᵢ)²)
    MANHATTAN,   ///< City-block distance Σ|aᵢ-bᵢ|
};

// =============================================================================
/// @brief Computes the distance between two equal-length vectors.
///
/// @details Uses either Euclidean or Manhattan distance.
///          Both vectors must have the same length.
///
/// @param a       First  vector (read-only reference — no copy)
/// @param b       Second vector (read-only reference — no copy)
/// @param metric  Which formula to use (default: EUCLIDEAN)
///
/// @return Distance as a non-negative double.
///
/// @throws std::invalid_argument  if @p a and @p b differ in size.
///
/// @note  For very large vectors, MANHATTAN is faster (no sqrt).
///
/// @par Example
/// @code
///   auto d = vectorDistance({1,2,3}, {4,6,3}, DistanceMetric::EUCLIDEAN);
///   // d == 5.0
/// @endcode
// =============================================================================
double vectorDistance(
    const std::vector<double>& a,
    const std::vector<double>& b,
    DistanceMetric metric = DistanceMetric::EUCLIDEAN)
{
    if (a.size() != b.size())
        throw std::invalid_argument("vectorDistance: vectors must be same size");

    double sum = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        sum += (metric == DistanceMetric::EUCLIDEAN) ? diff * diff : std::abs(diff);
    }
    return (metric == DistanceMetric::EUCLIDEAN) ? std::sqrt(sum) : sum;
}

// =============================================================================
/// @brief A minimal 2D matrix with basic operations.
///
/// Memory layout: row-major, stored in a flat 1D std::vector.
// =============================================================================
class Matrix {
public:
    /// @brief Construct a rows × cols matrix initialised to zero.
    /// @param rows  Number of rows    (must be > 0)
    /// @param cols  Number of columns (must be > 0)
    Matrix(std::size_t rows, std::size_t cols)
        : rows_(rows), cols_(cols), data_(rows * cols, 0.0)
    {}

    /// @brief Element access (mutable).
    /// @param r  Row index    (0-based)
    /// @param c  Column index (0-based)
    /// @throws std::out_of_range if indices are out of bounds
    double& at(std::size_t r, std::size_t c)
    {
        if (r >= rows_ || c >= cols_)
            throw std::out_of_range("Matrix::at — index out of range");
        return data_[r * cols_ + c];
    }

    /// @brief Element access (read-only).
    double at(std::size_t r, std::size_t c) const
    {
        if (r >= rows_ || c >= cols_)
            throw std::out_of_range("Matrix::at — index out of range");
        return data_[r * cols_ + c];
    }

    std::size_t rows() const { return rows_; }  ///< Row count
    std::size_t cols() const { return cols_; }  ///< Column count

    /// @brief Print the matrix to stdout.
    void print() const
    {
        for (std::size_t r = 0; r < rows_; ++r) {
            for (std::size_t c = 0; c < cols_; ++c)
                std::cout << at(r, c) << "\t";
            std::cout << "\n";
        }
    }

private:
    std::size_t          rows_, cols_;
    std::vector<double>  data_;  ///< Flat storage: data_[r * cols_ + c]
};

// =============================================================================
// INLINE COMMENTS — BEST PRACTICES
// =============================================================================
// BAD:  i++;              // increment i      ← states the obvious
// GOOD: i++;              // advance to next token
//
// BAD:  x = y * 0.0174533; // magic number
// GOOD: constexpr double DEG_TO_RAD = 3.14159265 / 180.0;
//       x = y * DEG_TO_RAD;
//
// Use TODO / FIXME / HACK / NOTE tags — IDEs highlight them:
// TODO(ashunigion): add batch insert support
// FIXME: overflow possible when N > INT_MAX/2
// HACK: temporary workaround for driver bug #1234
// NOTE: this must be called from the main thread only

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    // Distance demo
    std::vector<double> p1 = {1, 2, 3};
    std::vector<double> p2 = {4, 6, 3};

    double ed = vectorDistance(p1, p2, DistanceMetric::EUCLIDEAN);
    double md = vectorDistance(p1, p2, DistanceMetric::MANHATTAN);

    std::cout << "Euclidean : " << ed << "\n";  // 5
    std::cout << "Manhattan : " << md << "\n";  // 7

    // Matrix demo
    Matrix m(3, 3);
    m.at(0, 0) = 1;  m.at(0, 1) = 2;  m.at(0, 2) = 3;
    m.at(1, 0) = 4;  m.at(1, 1) = 5;  m.at(1, 2) = 6;
    m.at(2, 0) = 7;  m.at(2, 1) = 8;  m.at(2, 2) = 9;
    m.print();

    return 0;
}

// =============================================================================
// DOCUMENTATION SUMMARY
//
//  /// @brief  One-line description
//  /// @details  Longer explanation
//  /// @param  name  what it is
//  /// @return  what is returned
//  /// @throws  ExceptionType  when thrown
//  /// @note   extra info
//  /// @code / @endcode  — inline example
//
// STYLE GUIDES TO FOLLOW:
//  - Google C++ Style Guide   → https://google.github.io/styleguide/cppguide.html
//  - LLVM Coding Standards    → https://llvm.org/docs/CodingStandards.html
//  - CppCoreGuidelines (Bjarne) → https://isocpp.github.io/CppCoreGuidelines/
// =============================================================================
