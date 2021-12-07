#include <iostream>
#include <vector>
#include <unordered_set>

using Intvec = std::vector<int>;

void remove(Intvec &v) {
    // creating iterator starting with beginning of the vector 
    std::vector<int>::iterator itr = v.begin();
    std::unordered_set<int> s;
    // loops from the beginning to the end of the list 
    for (auto curr = v.begin(); curr != v.end(); ++curr) {
        if (s.insert(*curr).second) { // if the 0 curr already exist in the set
            *itr++ = *curr; // adding a position to the iterator 
        }
    }
    // erasing repeating positions in the set 
    v.erase(itr, v.end());
}


int main() {
    Intvec v = {1,2,3,4,5,5,6,6,7,3,4,5,1,4,6,3,3};
    remove(v);
    for (int i:v) {
        std::cout << " " << i;
    }
    return 0;
}