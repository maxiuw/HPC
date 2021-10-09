#include <iostream>
#include <vector>

using strVector = std::vector<std::string>;
//find the max word 
std::string find(const strVector& str){
    std::string max;
    for(std::string word:str){
        if(word>max){
            max = word;
        }
    }
    return max;
}
int main(){
    strVector vec {"mama","kozika","glupia","zwykla","sika"};
    std::cout << find(vec);
    return 0;
}