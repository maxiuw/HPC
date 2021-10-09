#include <vector>
#include <iostream>

int main(){
    std::vector<int> vct = {10,10,10,10};
    // print vals of vector each line 
    for(int i:vct){
        std::cout<<i<<' ';//<<std::endl;
    }

    return 0;

}