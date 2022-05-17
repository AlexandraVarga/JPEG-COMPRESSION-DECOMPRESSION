#include <vector>
#include <math.h>
#define pi acos(-1.0)
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

vector<vector<float>> dct(vector<vector<float>> imgMat)
{

    int height = imgMat.size();
    int width = imgMat[0].size();

    vector<vector<float>> compressed_image;

    float ci, cj, dct1, sum;

    for (int i = 0; i < height; i++)
    {
        vector<float> temp;
        for (int j = 0; j < width; j++)
        {

            if (i == 0)
                ci = 1 / sqrt(height);
            else
                ci = sqrt(2) / sqrt(height);
            if (j == 0)
                cj = 1 / sqrt(width);
            else
                cj = sqrt(2) / sqrt(width);

            sum = 0;
            for (int k = 0; k < height; k++)
            {
                for (int l = 0; l < width; l++)
                {
                    dct1 = imgMat[k][l] *
                           cos((2 * k + 1) * i * pi / (2 * height)) *
                           cos((2 * l + 1) * j * pi / (2 * width));
                    sum = sum + dct1;
                }
            }

            temp.push_back(ci * cj * sum);
        }
        compressed_image.push_back(temp);
    }

    return compressed_image;
}
