#include <algorithm>
#include <iostream>
#include <unordered_map>

//def StfIntMap as data type
using StfIntMap = std::unordered_map<std::string, int>; //key is string,val is int



int main(){
    const StfIntMap ex = {{"one",1}, {"two",2}};

    for(auto entry:ex){
        std::cout << entry.first << " val " << entry.second<<std::endl;
    }
    // new input - if map is NOT cosnt
    // if it is cosntant -> we need to use 'add' method
    // ex["kozik"] = 2;
    //ex.insert({"kozik",2});


    return 0;

}