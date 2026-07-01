#include <iostream>
#include <vector>
#include <limits>

int main(){
    int ts;
   //getting total number of test sets
    std::cin >> ts;
    std::cin.ignore();

    for(int tc=1; tc<=ts;tc++){
        std::string str;
        std::getline(std::cin,str);
        int c=str.length();
        if (c>10){
            char f=str[0];
            char l=str[c-1];
            std::cout<<std::string(1,f)+std::to_string(c-2)+std::string(1,l)<< "\n";
        }else{
            std::cout << str << "\n";
        };
    }
    return 0;
}