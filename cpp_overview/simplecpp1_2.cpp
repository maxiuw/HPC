#include <iostream>
#include <string>

int main(){
    int i = 10;
    std::string s = std::to_string(i); // to string conv
    i = std::stoi(s) + 10; // string to int
    std::cout << "i = "<< i << std::endl;
    return 0; 
}