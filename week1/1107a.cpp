#include <iostream>
#include <vector>
#include <limits>

int main(){
    int ts;
   //getting total number of test sets
    std::cin >> ts;

    for(int tc=1; tc<=ts;tc++){
        int x, y;
        std::cin >>x >>y;

        if (x%y==0){
            std::cout << "YES\n";
        } else{
            std::cout << "NO\n";
        }
    }
    return 0;
}