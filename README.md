# JPEG-Compression-Decompression

Process Of JPEG Compression : 
Firstly, we convert the R, G, B color format to Y, Cb, Cr format.
Some colors are more sensitive to human eyes and thus are high-frequency colors.
Some colors of chromium compounds like Cb and Cr are less sensitive to human eyes thus can be ignored. Then we reduce the size of pixels in downsampling.
We divide our image into 8*8 pixels and perform forward DCT(Direct Cosine Transformation).
Then we perform quantization using quantum tables and we compress our data using various encoding methods like run-length encoding and Huffman encoding. 

In the second stage, we decompress our data, It involves decoding where we decode our data, and we again de-quantize our data by referring to the quantization table.
Then we perform Inverse DCT and upsampling to convert it into original pixels and finally, color transformation takes place to convert the image into its original color format. 
