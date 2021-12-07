// Given  a  list  of  unsorted  numbers,  find  if  any  two  numbers  sum  up  to  a  given  value. 
// For  example,  given  the  list  {1,  4,  3,  2,  4,  1}  and  value  8,  the  method  should  return 
// true, because 4 + 4 == 8. This should be a O(n) solution for full points. 
#include <iostream>
#include <unordered_map>
#include <vector>

using Intvec = std::vector<int>;
using Intmap = std::unordered_map<int,int>;


bool findsum(Intvec &vec, int sum) {
    Intmap vals;
    for (int i = 0; i < vec.size(); i++) {
        vals[vec[i]] = vec[i];

        if (vals[sum - vec[i]] == sum - vec[i])
            return true;
    }
    return false;
}
int main() {
    Intvec vec = {12,3,3,3,12,5,31,45,31,314,1,32,23,51,34};
    std::cout << findsum(vec,4453);
    return 0;
}