#include <vector>
#include <math.h>
#define pi acos(-1.0)
using namespace std;

#define vi vector<int>
#define vmati vector<vector<int>>

vi zigzag(vmati matrix)
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

    float ci, cj, temp_dct, sum;

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
                    temp_dct = imgMat[k][l] *
                               cos((2 * k + 1) * i * pi / (2 * height)) *
                               cos((2 * l + 1) * j * pi / (2 * width));
                    sum += temp_dct;
                }
            }

            temp.push_back(ci * cj * sum);
        }
        compressed_image.push_back(temp);
    }

    return compressed_image;
}

vector<vector<float>> idct(vector<vector<float>> compressed_img)
{
    int height = compressed_img.size();
    int width = compressed_img[0].size();

    vector<vector<float>> decompressed_img;

    float ci, cj, temp_idct, sum;

    for (int i = 0; i < height; i++)
    {
        vector<float> temp;
        for (int j = 0; j < width; j++)
        {

            sum = 0;
            for (int k = 0; k < height; k++)
            {
                for (int l = 0; l < width; l++)
                {
                    if (k == 0)
                        ci = 1 / sqrt(2);
                    else
                        ci = 1;
                    if (j == 0)
                        cj = 1 / sqrt(2);
                    else
                        cj = 1;
                    temp_idct = compressed_img[k][l] *
                                cos((2 * i + 1) * k * pi / (2 * height)) *
                                cos((2 * j + 1) * l * pi / (2 * width));
                    sum += temp_idct;
                }
            }

            temp.push_back(ci * cj * sum);
        }
        decompressed_img.push_back(temp);
    }

    return decompressed_img;
}
