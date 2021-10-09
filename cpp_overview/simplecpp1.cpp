#include <iostream>
#include <iomanip>

int main(){
    int number = 0;
    std::string word = ""; // def string, aviod global vars!!!! 
    std::cout << "enter numb and the word"; // << output operator, cout - output stream!
    std::cin >> number >> std::quoted(word); 
    // >> stream extraction operator, input operator, spaces are important
    // std::quoted is taking care of multiple words entered at the saem time
    std::cout << "the word is " << word << "has " << word.size()
        << "letters \nThe number is: " << number << std::endl;

    return 0;

}
