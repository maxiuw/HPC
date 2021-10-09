#include <iostream>
#include<vector>

class WordPair {
public:

    std::string word1; // init params 
    std::string word2;

}; 
std::vector<WordPair> phrases;

WordPair read(std::istream& is){
    WordPair wp;
    is >> wp.word1 >> wp.word2;
    return wp;
}

int main(){
    // const std::string st = std::cin; 
    WordPair out = read(std::cin);
    std::cout << out.word1 << out.word2;
    return 0;
}
// now we can use is as any class



