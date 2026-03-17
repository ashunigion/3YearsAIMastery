// =============================================================================
// mathutils.cpp — Implementation file (definitions)
// =============================================================================

#include "mathutils.h"   // Always include your own header first
#include <stdexcept>     // std::invalid_argument
#include <numeric>       // std::accumulate

namespace MathUtils {

    int factorial(int n) {
        if (n < 0) throw std::invalid_argument("factorial: n must be >= 0");
        return (n <= 1) ? 1 : n * factorial(n - 1);
    }

    double mean(const std::vector<double>& data) {
        if (data.empty()) throw std::invalid_argument("mean: empty vector");
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        return sum / static_cast<double>(data.size());
    }

    bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i)
            if (n % i == 0) return false;
        return true;
    }

}  // namespace MathUtils
