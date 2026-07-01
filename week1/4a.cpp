#include <iostream>
#include <vector>
#include <limits>

int main(){
    int ts;
   //getting total number of test sets
    std::cin >> ts;

    if ((ts>2)&&(ts%2==0)){
        std::cout << "YES\n";
    } else{
        std::cout << "NO\n";
    }

    return 0;
}