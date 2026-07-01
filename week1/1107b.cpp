#include <iostream>
#include <vector>
#include <limits>

bool goodNum(long long n){
    int d1=-1;
    int d2=-1;
    while (n>0){
        int temp=n%10;
        if (d1==-1 || d1==temp){
            d1=temp;
            //std::cout<<d1<<"\n";

            n=n/10;
            //std::cout<<n<<"\n";
        }else if(d2==-1 || d2==temp){
            d2=temp;
            //std::cout<<d2<<"\n";
            n=n/10;
            //std::cout<<n<<"\n";
        }else{
            return false;
        }

    }
    return true;
}

// std::vector<long long> generateGoodNumbers(){
//     std::vector<long long> goodNums;
// }

int main(){
    //std::vector<long long> goodNums = generateGoodNumbers();
    int ts;
   //getting total number of test sets
    std::cin >> ts;

    for(int tc=1; tc<=ts;tc++){
        long long x;
        std::cin >>x;

        for (int j=2; j<=100000; j++){
            if (goodNum(j) && goodNum((long long)j*x)){
                std::cout << j <<"\n";
                break;
            }
        }
    }

    return 0;
}