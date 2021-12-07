#include <iostream>
#include <vector>
#include <unordered_map>

using IntVect = std::vector<int>;
using IntMap = std::unordered_map<int,int>;
IntVect inputvect = {1, -4, 2, 3, -2, 10, -3, 100};

int main() {
    IntMap maxforindex; 
    int maxsum; 
    IntVect solution = {0,0}; 
    for (int idx = 0; idx < inputvect.size(); idx++) {
        maxforindex[idx] = 0;     
        for (int key = 0; key < maxforindex.size(); key++) {
            int addition = inputvect[idx] + maxforindex[key];
            if (addition > maxsum) { 
                maxsum = addition;
                solution = {key, idx};           
            }
            maxforindex[key] += addition;
        }     
    }
    std::cout << solution[0] << solution[1];
}