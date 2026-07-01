#include <iostream>
#include <vector>
#include <limits>

int main(){
    int n,k,t;
   //getting total number of test sets
    std::cin >> n >> k >> t;
    int r=(t*n)/100;
    int x=(t*n*k/100)-(r*k);
    for(int i=1;i<=r;i++){
        std::cout<<k<<" ";
    }
    if (r<n)
        std::cout<<x<<" ";
    for(int i=r+2;i<=n;i++){
        std::cout<<0<<" ";
    }

    return 0;
}