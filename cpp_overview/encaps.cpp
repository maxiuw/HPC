#include <iostream>

//private encapsulation - we cannot directly access the values
class WordPair{
    public:
    // non-arugment constructor
    WordPair(){
        word1 = word2 = "";
    }
    WordPair(const std::string& w1, const std::string& w2){
        word1 = w1;
        word2 = w2;
    }
    // destructor 
    ~WordPair(){} //cleans memo

    // getter method 
    // defined as const method, not meant to modify anything
    std::string get(bool w1 = true) const {
        return w1 ? word1 : word2;
    }



    // private, cannot be changed 
    private:
    std::string word1;
    std::string word2;


};

int main(){
    WordPair p = WordPair("a","b");
    std::cout << p.get();

    return 0;
}