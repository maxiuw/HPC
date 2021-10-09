#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>
#include<algorithm>

using Net = std::unordered_map<std::string, std::vector<std::string>>;

Net buildnet(const size_t &nodes)
{
    // Net net = {{"A",{"B","C"}}};
    Net net;

    for (int i = 0; i < nodes; i++)
    {
        char key = 'a' + i; // rand() % 26;
        std::cout << "key " << key << std::endl;
        std::string s(1, key);
        net[s] = {};
    }
    
    for (int j = 0; j<nodes; j++)
    {
        int n_neigh = rand()%nodes-1;
        // org node 
        std::string orgnode = "a" + j;
        for (int n = 0; n < n_neigh; n++){
            
            std::string key = "a" + n;
            // to make sure w do not duplicate keys 
            if (std::find(net[orgnode].begin(),net[orgnode].end(),key) == net[orgnode].end())
            {
                std::cout << orgnode << " <--> " << key << std::endl;
                net[orgnode].push_back(key);
                net[key].push_back(orgnode);
            }

        }
    }
        
        
    return net;
}

int main()
{   
    const size_t n_nodes = 10;
    Net net = buildnet(n_nodes);
    // Net net = {{"A", {"B", "C"}}};
    for (const auto &n :net["a"]){
        std::cout << n << std::endl;
    }
        
    // for(auto& node:net){
 
    //     std::cout << node.first <<std::endl;

    // }
    return 0;
}
