#include <iostream>
#include <iomanip>

int main(){
    std::string element, symbol;
    int atWt;
    // as long as i get input, do operation inside the loop
    while (std::cin >> "inpt stuff ">> std::quoted(element)>>atWt>>symbol)
        std::cout << element;
    return 0;
}