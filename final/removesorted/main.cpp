#include <iostream>
#include <vector>
#include <algorithm>    // std::sort

using Intvec = std::vector<int>;

Intvec remove(Intvec& list, int torem) {
    std::sort(list.begin(), list.end());
     for (int j:list) {
        std::cout<<" "<<j;
    }
    int start = -1;
    int stop = -1;

    for (int i = 0; i < list.size(); i++) {
        if (list[i] == torem) {
            if  (start < 0)
                start = i;
            else {
                stop = i;
            }
        }
        else if (list[i] > torem) {
            break;
        }
    }
    if (start < 0) {
        return list;
    } 
    std::cout << start << stop;
    list.erase(list.begin() + start, list.begin() + stop+1);
    return list;
}
int main() {
    Intvec list = {1,2,3,444,4,3,22,1,2,3,3,4,55,4,5,5,6,3,22,3};
    list = remove(list,3);
    for (int j:list) {
        std::cout<<" "<<j;
    }
}