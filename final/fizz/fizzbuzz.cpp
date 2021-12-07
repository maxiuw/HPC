#include <iostream>
#include <vector>
#include <unordered_map>

using IntVect = std::vector<int>;
using IntStrMap = std::unordered_map<std::string,int>;

IntStrMap fizzbuzz(IntVect& fizz) {
    IntStrMap result;
    result["fizz"] = 0;
    result["buzz"] = 0;
    result["fizzbuzz"] = 0;
    for (int i = 0; i < fizz.size(); i++) {
        if (i%3 == 0) result["fizz"]+=1;
        if (i%5 == 0) result["buzz"]+=1;
        if (i%15 == 0) result["fizzbuzz"]+=1;
    }
    return result;
}

int main() {
    IntVect fizz = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    IntStrMap resutls = fizzbuzz(fizz);
    return resutls["buzz"];
}
