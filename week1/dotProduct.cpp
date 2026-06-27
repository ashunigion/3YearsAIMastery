#include <iostream>
#include <vector>
#include <array>

int main(){
    int raw[5] = {1,2,3,4,5};

    std::cout << raw[3] << "\n";

    std::array<int, 5> arr = {8,9,10,11,12};
    std::cout << arr[1] << "\n";

    std::vector<int> v = {13,14,15};
    std::cout << v[2] << "\n";

    std::vector<std::vector<int>> vi = {
        {1,2,3,4},
        {5,6,7,8}
    };

    std::vector<std::vector<int>> vj = {
        {8,9},
        {14,15},
        {5,6},
        {10,15}
    };
    
    int virows = vi.size();
    int vicolumns = vi[0].size();

    int vjrows = vj.size();
    int vjcolumns = vj[0].size();

    std::cout << "number of rows in vi is " << virows << "\n";
    std::cout << "number of columns in vi is " << vicolumns << "\n";

    std::cout << "number of rows in vj is " << vjrows << "\n";
    std::cout << "number of columns in vj is " << vjcolumns << "\n";

    int voutrows = virows;
    int voutcolums = vjcolumns;
    std::vector<std::vector<int>> vout;
    
}