#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


#include <cstdio>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <cstring>
#include "common.h"
#include <random>
#include <fstream>
using namespace std;
using namespace cv;

// matrix of luminance data
double luminanceData[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};
// matrix of chrominance data
double chrominanceData[8][8] = {
    {17, 18, 24, 27, 99, 99, 99, 99},
    {18, 21, 26, 66, 99, 99, 99, 99},
    {24, 26, 56, 99, 99, 99, 99, 99},
    {47, 66, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99}
};

Mat compression(Mat originalImage) {

    // Get the height and width of the original image
    int height = originalImage.size().height;
    int width = originalImage.size().width;

    // Convert the image to YUV
    Mat convertedImage;
    cvtColor(originalImage, convertedImage, COLOR_RGB2YUV);// Y U V

    // Transform 2D array of luminance and chrominance to matrix
    Mat luminance = Mat(8, 8, CV_64FC1, &luminanceData);
    Mat chrominance = Mat(8, 8, CV_64FC1, &chrominanceData);

    // split the image to 3 planes because of ( Red - Green - Blue ) channels
    vector<Mat> planes;
    split(convertedImage, planes);

    // get the number of channels (planes) {3}
    int numberOfPlanes = convertedImage.channels();

    // modify the height and width of image to make it divide by 8
    height = height - height % 8;
    width = width - width % 8;

    for (int i = 0; i < height; i += 8) {

        for (int j = 0; j < width; j += 8) {

            for (int plane = 1; plane < numberOfPlanes; plane++) {

                // get a block8x8 of size 8x8
                Mat block8x8 = planes[plane](Rect(j, i, 8, 8));

                // convert the block to float(to get the value of quantization)
                block8x8.convertTo(block8x8, CV_64FC1);

                // Subtract the block by 128 to prepare it for the DCT
                subtract(block8x8, 128.0, block8x8);

                // Apply DCT
                dct(block8x8, block8x8);//implementare manuala

                // Apply quantization - why is different?
                if (plane == 0) {// for Y channel - correspondent intensity
                    divide(block8x8, luminance, block8x8);
                }
                else {//color - using U, V channels
                    divide(block8x8, chrominance, block8x8);
                }
                // Convert it back to unsigned int
                block8x8.convertTo(block8x8, CV_8UC1);

                // save the modified block to the new image
                block8x8.copyTo(planes[plane](Rect(j, i, 8, 8)));
            }
        }
    }
    Mat finalImage;
    // Merge the 3 channels to the final image
    merge(planes, finalImage);
    return finalImage;
}


// Decompression
Mat decompression(Mat finalImage) {
    // Get the height and width of the original image
    int height = finalImage.size().height;
    int width = finalImage.size().width;

    // Transform 2D array of luminance and chrominance to matrix
    Mat luminance = Mat(8, 8, CV_64FC1, &luminanceData);
    Mat chrominance = Mat(8, 8, CV_64FC1, &chrominanceData);

    // split the image to 3 planes because work on color image ( Red - Green - Blue )
    vector<Mat> planes;
    split(finalImage, planes);

    // get the number of channels (planes) {3}
    int numberOfPlanes = finalImage.channels();

    // modify the height and width of image to make it divide by 8
    height = height - height % 8;
    width = width - width % 8;

    for (int i = 0; i < height; i += 8) {

        for (int j = 0; j < width; j += 8) {

            for (int plane = 1; plane < numberOfPlanes; plane++) {

                // get a block of size 8x8
                Mat block8x8 = planes[plane](Rect(j, i, 8, 8));

                // convert the block to float(to get the value of quantization)
                block8x8.convertTo(block8x8, CV_64FC1);

                // Apply dequantization ( multiplying)
                if (plane == 0) {
                    multiply(block8x8, luminance, block8x8);
                }
                else {
                    multiply(block8x8, chrominance, block8x8);
                }

                // Apply IDCT
                idct(block8x8, block8x8);//manual

                // Add 128 to the block
                add(block8x8, 128.0, block8x8);

                // Convert it back to unsigned int
                block8x8.convertTo(block8x8, CV_8UC1);

                // After applying everything of the algorithm we will save the block to the original Image.
                block8x8.copyTo(planes[plane](Rect(j, i, 8, 8)));
            }
        }
    }

    // merge the planes with the final Image
    merge(planes, finalImage);

    cvtColor(finalImage, finalImage, COLOR_YUV2RGB);
    return finalImage;
}



int main() {
    
        char *file_name = "BMP/img-1.bmp";
        // Read the image
        Mat image = imread(file_name, IMREAD_COLOR);

        // check if the image is valid or not
        if (image.empty()) {
            cout << "Could not open or find the image" << endl;
            return -1;
        }
        // get the size of the original image
        ifstream fin(file_name, ios::binary);
        fin.seekg(0, ios_base::end);
        streampos originalImageSize = fin.tellg();
        fin.close();

        // compress the image and save it to compressedImage
        Mat compressedImage = compression(image);
        imwrite("compressedImage.jpg", compressedImage);

        // get the size of the compressed image
        ifstream fin1("compressedImage.jpg", ios::binary);
        fin1.seekg(0, ios_base::end);
        streampos CompressedImageSize = fin1.tellg();
        fin1.close();

        // to save the compressed to binary file
        //FileStorage fs("file.yml", FileStorage::WRITE);
        //fs << "yourMat" << compressedImage;

        // Decompress the image and save it
        Mat uncompressedImage = decompression(compressedImage);
        imwrite("DecompressedImage.jpg", uncompressedImage);
        
        //showing the results before and after compression
        //imshow("Before compression", image);
        imshow("Decompressed image", uncompressedImage);
        waitKey(0);
        
        // calculate the compression ratio
        double compressionRatio = (double)CompressedImageSize / (double)originalImageSize;
        cout << "Compression Ratio: " << compressionRatio << endl;
        double savedSpace = 1 - compressionRatio;
        cout <<fixed<<setprecision(2)<< "Saved space:" << savedSpace * 100 << "%" << endl;

    
    return 0;
}
