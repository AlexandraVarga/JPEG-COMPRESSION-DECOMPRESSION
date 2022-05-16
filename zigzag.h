#include <vector>
using namespace std;

#define vi vector<int>
#define vmati vector<vector<int>>

vector<int> zigzag(vmati matrix)
{
    int length = matrix.size();
    int width = matrix[0].size();
    vi ans;
    for (int i = 0; i < length; i++)
    {
        if (i % 2 == 0)
        {
            for (int j = 0; j <= i; j++)
                ans.push_back(matrix[j][i - j]);
        }
        else
        {
            for (int j = i; j >= 0; j--)
                ans.push_back(matrix[j][i - j]);
        }
    }
    return ans;
}
