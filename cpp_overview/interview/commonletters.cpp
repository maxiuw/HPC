#include <iostream>
#include <vector>


int commonCharacterCount(std::string s1, std::string s2) {
    std::vector<int> s1count(26, 0);        
    std::vector<int> s2count(26, 0);   

        for (int i:s1count){
            std::cout << i << std::endl;
        }
        for (int i = 0; i<s1.length();i++){
            //value of 'a' in ascii 97, 'z' - 122
            int idx = s1[i]-'a';
            s1count[idx]+=1;
        }
        
        for (int i = 0; i<s2.length();i++){
            //value of 'a' in ascii 97, 'z' - 122
            s2count[s2[i]-'a']+=1;
        }
        // for(int i:s1count){
        //     std::cout << i << std::endl;
        // }
        int repeat = 0;
        // compare and choose min occurency     
        for(int i = 0; i<26;i++){
            // std::cout <<(s1count[i],s2count[i])<<std::endl;

            repeat += (std::min(s1count[i],s2count[i]));
        }
        
    
    return repeat;

}
    
int main(){
    std::cout << commonCharacterCount("aabcc","adcaa");
    return 0;
}
    
    

