#include <iostream>
#include <vector>
#include <limits>

int main(){
    int ts;
    std::cout << "enter total number of steps: ";
    std::cin >> ts;

    // check for decimal input
    if (std::cin.peek() == '.'){
        std::cout << "Error: Decimal values not allowed. Please enter a whole number.";
        return 0;
    }

    // clear the rest of the line
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    std::vector<long long> s(ts+1, 0);

    // test for string and non positive numbers
    if (ts<=0){
        std::cout << "step count not acceptable";
        return 0;
    }
    else if (ts==1){
        std::cout << "number of ways " << 1;
        return 0;
    }
    else if(ts==2){
        std::cout << "number of ways " << 2;
        return 0;
    }
    else if(ts==3){
        std::cout << "number of ways " << 4;
        return 0;
    } 
/*
    s[0]=0;
    s[1]=1;
    s[2]=2;
    s[3]=4;*/
    long long a=1, b=2, c = 4, d;

    int i = 4;
    for(;i<=ts;i++){
        //s[i]=((s[i-1])+(s[i-2])+(s[i-3]));
        d = a+b+c;
        a=b;
        b=c;
        c=d;

    }
    //std::cout << "number of ways " << s[i-1];
    std::cout << "number of ways " <<c; 

    return 0;
}