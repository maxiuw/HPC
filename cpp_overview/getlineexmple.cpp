#include <iostream>
#include <string>

int main(){
    std::cout << "enter a line: ";
    std::string line; // an empty string 
    std::getline(std::cin, line); //getline reads characters from an input stream and places them into a string
    std::cout << "you inputed " << line<<std::endl;
    const std::string firstWord = line.substr(0,line.find(' ')); // substring everything which is after the line.find('') idx
    std::cout << "first w " << firstWord << "has "<<line.find(' ')-1 << "chars" << std::endl;
    const std::string lastW = line.substr(line.rfind(' ')); // do not need the ending idx, if just the first idx is provided then idx till the end 
    std::cout << "last W " << lastW; 

    return 0;
}