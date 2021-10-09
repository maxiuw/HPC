#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

int main(){
    std::ifstream f ("bla.txt");
    std::string word,word1;
    while (f>>word>>word1) {
        std::cout << word <<"/"<<word1<< '\n';
    }
    return 0;
}