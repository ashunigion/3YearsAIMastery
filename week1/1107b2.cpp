#include <iostream>
#include <string>

// Problem "Good times Good times":
//   x is good (<= 2 distinct digits), 1 <= x < 1e8.
//   Find good y, 2 <= y <= 1e9, such that x*y is also good.
//
// Closed-form construction (O(1) per query, no search):
//   Let k = number of digits of x. Output y = 10^k + 1.
//   Then x*y = x*10^k + x. Since x has exactly k digits, x*10^k shifts x left by
//   k places and the low k digits are x again -> the result is the string "x"
//   concatenated with itself. Concatenation reuses x's digit set, so x*y has the
//   same (<= 2) distinct digits as x and is good.
//   y = 10^k + 1 is "1" + (k-1) zeros + "1", digits {1,0} -> good, and since k <= 8
//   we have 11 <= y <= 1e8+1 < 1e9. So both conditions always hold.
//
// y = 10^k + 1 is printed directly as a string to avoid any overflow and stay fast.

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    std::string out;
    while(t--){
        std::string x;
        std::cin >> x;        // read x as text; its length is k
        int k = (int)x.size();

        // y = 10^k + 1 = '1' followed by (k-1) '0's followed by '1'
        out += '1';
        out.append(k - 1, '0');
        out += '1';
        out += '\n';
    }
    std::cout << out;
    return 0;
}
