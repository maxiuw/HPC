#include<algorithm>
#include<vector>
int houseRobber(std::vector<int> nums) {
    int x = 0, y = 0;
    for (int n : nums) {
        int z = std::max(x+n,y);
        x = y;
        y = z;
    }
    return y;
}
    