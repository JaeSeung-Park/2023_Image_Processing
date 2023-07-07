#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("test3.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	//´Ù¿î»ùÇÃ¸µ
	int ratio = 2;
	int width2 = bmpInfo.biWidth >> ratio;
	int height2 = bmpInfo.biHeight >> ratio;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	int size2 = stride2 * height2;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width2, stride2, height2, size2, bitCnt);

	////¾÷»ùÇÃ¸µ
	//int ratio1 = 0;
	//int width3 = bmpInfo.biWidth << ratio1;
	//int height3 = bmpInfo.biHeight << ratio1;
	//int stride3 = (((bitCnt / 8) * width) + 3) / 4 * 4;
	//int size3 = stride3 * height3;
	//printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride3, height3, size3, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL, * outputImg1 = NULL, * outputImg2 = NULL;

	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg1 = (unsigned char*)calloc(size2, sizeof(unsigned char));
	outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);

	printf("1111ÇÒ´ç\n");
	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));
	unsigned char* Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));


	double Y;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			Y1[j * width + i] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
		}
	}

	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		Y1[j * width + i] = inputImg[j * stride + 3 * i + 2];
	//	}
	//}

	printf("2222´Ù¿î»ù\n");
	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++)
		{
			Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)];

		}
	}
	int p, q;
	printf("3333¾÷»ù\n");
	for (int j = 0; j < height2 - 1; j++)
	{
		for (int i = 0; i < width2 - 1; i++)
		{
			p = j * 4;
			q = i * 4;
			Y3[p * width + q] = Y2[j * width2 + i];
			Y3[p * width + q + 1] = (3 * Y2[j * width2 + i] + 1 * Y2[j * width2 + i + 1]) / 4.0;
			Y3[p * width + q + 2] = (2 * Y2[j * width2 + i] + 2 * Y2[j * width2 + i + 1]) / 4.0;
			Y3[p * width + q + 3] = (1 * Y2[j * width2 + i] + 3 * Y2[j * width2 + i + 1]) / 4.0;
			Y3[p * width + q + 4] = Y2[j * width2 + i + 1];

			Y3[(p + 1) * width + q] = (3 * Y2[j * width2 + i] + 1 * Y2[(j + 1) * width2 + i]) / 4.0;
			Y3[(p + 2) * width + q] = (2 * Y2[j * width2 + i] + 2 * Y2[(j + 1) * width2 + i]) / 4.0;
			Y3[(p + 3) * width + q] = (1 * Y2[j * width2 + i] + 3 * Y2[(j + 1) * width2 + i]) / 4.0;
			Y3[(p + 4) * width + q] = Y2[(j + 1) * width2 + i];

			Y3[(p + 1) * width + q + 4] = (3 * Y2[j * width2 + i + 1] + 1 * Y2[(j + 1) * width2 + i + 1]) / 4.0;
			Y3[(p + 2) * width + q + 4] = (2 * Y2[j * width2 + i + 1] + 2 * Y2[(j + 1) * width2 + i + 1]) / 4.0;
			Y3[(p + 3) * width + q + 4] = (1 * Y2[j * width2 + i + 1] + 3 * Y2[(j + 1) * width2 + i + 1]) / 4.0;
			Y3[(p + 4) * width + q + 4] = Y2[(j + 1) * width2 + i + 1];

			Y3[(p + 4) * width + q + 1] = (3 * Y2[(j + 1) * width2 + i] + 1 * Y2[(j + 1) * width2 + i + 1]) / 4.0;
			Y3[(p + 4) * width + q + 2] = (2 * Y2[(j + 1) * width2 + i] + 2 * Y2[(j + 1) * width2 + i + 1]) / 4.0;
			Y3[(p + 4) * width + q + 3] = (1 * Y2[(j + 1) * width2 + i] + 3 * Y2[(j + 1) * width2 + i + 1]) / 4.0;
			//---------------------------------------------------------------------------------------------------

			//Y3[(p + 1) * width3 + q + 1] =
			//	(2 * Y2[j * width2 + i] + 1 * Y2[j * width2 + i + 1] + 2 * Y2[(j + 1) * width2 + i] + 1 * Y2[(j + 1) * width2 + i + 1]) / 6.0;
			//Y3[(p + 1) * width3 + q + 2] =
			//	(1 * Y2[j * width2 + i] + 2 * Y2[j * width2 + i + 1] + 2 * Y2[(j + 1) * width2 + i] + 1 * Y2[(j + 1) * width2 + i + 1]) / 6.0;
			//Y3[(p + 2) * width3 + q + 1] =
			//	(2 * Y2[j * width2 + i] + 1 * Y2[j * width2 + i + 1] + 1 * Y2[(j + 1) * width2 + i] + 2 * Y2[(j + 1) * width2 + i + 1]) / 6.0;
			//Y3[(p + 2) * width3 + q + 2] =
			//	(1 * Y2[j * width2 + i] + 2 * Y2[j * width2 + i + 1] + 1 * Y2[(j + 1) * width2 + i] + 2 * Y2[(j + 1) * width2 + i + 1]) / 6.0;

			//Y3[(p + 1) * width3 + q + 1] = (3 * Y3[p * width3 + q + 1] + 2 * Y3[(p + 3) * width3 + q + 1]) / 5.0;
			//Y3[(p + 1) * width3 + q + 2] = (3 * Y3[p * width3 + q + 2] + 2 * Y3[(p + 3) * width3 + q + 2]) / 5.0;
			//Y3[(p + 2) * width3 + q + 1] = (2 * Y3[p * width3 + q + 1] + 3 * Y3[(p + 3) * width3 + q + 1]) / 5.0;
			//Y3[(p + 2) * width3 + q + 2] = (2 * Y3[p * width3 + q + 2] + 3 * Y3[(p + 3) * width3 + q + 2]) / 5.0;

			Y3[(p + 1) * width + q + 1] =
				(2 * Y3[(p + 1) * width + q] + 2 * Y3[(p + 1) * width + q + 4]
					+ 3 * Y3[p * width + q + 1] + 1 * Y3[(p + 4) * width + q + 1]) / 8.0;
			Y3[(p + 1) * width + q + 2] =
				(2 * Y3[(p + 1) * width + q] + 2 * Y3[(p + 1) * width + q + 4]
					+ 3 * Y3[p * width + q + 2] + 1 * Y3[(p + 4) * width + q + 2]) / 8.0;
			Y3[(p + 1) * width + q + 3] =
				(2 * Y3[(p + 1) * width + q] + 2 * Y3[(p + 1) * width + q + 4]
					+ 3 * Y3[p * width + q + 3] + 1 * Y3[(p + 4) * width + q + 3]) / 8.0;

			Y3[(p + 2) * width + q + 1] =
				(2 * Y3[(p + 2) * width + q] + 2 * Y3[(p + 2) * width + q + 4]
					+ 2 * Y3[p * width + q + 1] + 2 * Y3[(p + 4) * width + q + 1]) / 8.0;
			Y3[(p + 2) * width + q + 2] =
				(2 * Y3[(p + 2) * width + q] + 2 * Y3[(p + 2) * width + q + 4]
					+ 2 * Y3[p * width + q + 2] + 2 * Y3[(p + 4) * width + q + 2]) / 8.0;
			Y3[(p + 2) * width + q + 3] =
				(2 * Y3[(p + 2) * width + q] + 2 * Y3[(p + 2) * width + q + 4]
					+ 2 * Y3[p * width + q + 3] + 2 * Y3[(p + 4) * width + q + 3]) / 8.0;

			Y3[(p + 3) * width + q + 1] =
				(2 * Y3[(p + 3) * width + q] + 2 * Y3[(p + 3) * width + q + 4]
					+ 1 * Y3[p * width + q + 1] + 3 * Y3[(p + 4) * width + q + 1]) / 8.0;
			Y3[(p + 3) * width + q + 2] =
				(2 * Y3[(p + 3) * width + q] + 2 * Y3[(p + 3) * width + q + 4]
					+ 1 * Y3[p * width + q + 2] + 3 * Y3[(p + 4) * width + q + 2]) / 8.0;
			Y3[(p + 3) * width + q + 3] =
				(2 * Y3[(p + 3) * width + q] + 2 * Y3[(p + 3) * width + q + 4]
					+ 1 * Y3[p * width + q + 3] + 3 * Y3[(p + 4) * width + q + 3]) / 8.0;

		/*	Y3[(p + 2) * width + q + 2] =
				(2 * Y3[(p + 2) * width + q + 1] + 2 * Y3[(p + 2) * width + q + 3]
					+ 2 * Y3[(p + 1) * width + q + 2] + 2 * Y3[(p + 3) * width + q + 2]) / 8.0;*/

	    /*	Y3[(p + 2) * width + q + 2] = (Y2[j * width2 + i] + Y2[j * width2 + i + 1]
				+ Y2[(j + 1) * width2 + i] + Y2[(j + 1) * width2 + i + 1]) / 4.0;*/


			//-------------------------------------------------------------------------------------------------------
			

			//-------------------------------------------------------------------------------------------------
			//p = j * 8;
			//q = i * 8;
			//Y3[p * width3 + q] = Y2[j * width2 + i];
			//Y3[p * width3 + q + 1] = (7 * Y2[j * width2 + i] + 1 * Y2[j * width2 + i + 1]) / 8.0;
			//Y3[p * width3 + q + 2] = (6 * Y2[j * width2 + i] + 2 * Y2[j * width2 + i + 1]) / 8.0;
			//Y3[p * width3 + q + 3] = (5 * Y2[j * width2 + i] + 3 * Y2[j * width2 + i + 1]) / 8.0;
			//Y3[p * width3 + q + 4] = (3 * Y2[j * width2 + i] + 5 * Y2[j * width2 + i + 1]) / 8.0;
			//Y3[p * width3 + q + 5] = (2 * Y2[j * width2 + i] + 6 * Y2[j * width2 + i + 1]) / 8.0;
			//Y3[p * width3 + q + 6] = (1 * Y2[j * width2 + i] + 7 * Y2[j * width2 + i + 1]) / 8.0;
			//Y3[p * width3 + q + 7] = (0 * Y2[j * width2 + i] + 8 * Y2[j * width2 + i + 1]) / 8.0;

			//Y3[(p + 1) * width3 + q] = (7 * Y2[j * width2 + i] + 1 * Y2[(j + 1) * width2 + i]) / 8.0;
			//Y3[(p + 2) * width3 + q] = (6 * Y2[j * width2 + i] + 2 * Y2[(j + 1) * width2 + i]) / 8.0;
			//Y3[(p + 3) * width3 + q] = (5 * Y2[j * width2 + i] + 3 * Y2[(j + 1) * width2 + i]) / 8.0;
			//Y3[(p + 4) * width3 + q] = (3 * Y2[j * width2 + i] + 5 * Y2[(j + 1) * width2 + i]) / 8.0;
			//Y3[(p + 5) * width3 + q] = (2 * Y2[j * width2 + i] + 6 * Y2[(j + 1) * width2 + i]) / 8.0;
			//Y3[(p + 6) * width3 + q] = (1 * Y2[j * width2 + i] + 7 * Y2[(j + 1) * width2 + i]) / 8.0;
			//Y3[(p + 7) * width3 + q] = (0 * Y2[j * width2 + i] + 8 * Y2[(j + 1) * width2 + i]) / 8.0;
			//					  
			//Y3[(p + 1) * width3 + q + 7] = (7 * Y2[j * width2 + i + 1] + 1 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 2) * width3 + q + 7] = (6 * Y2[j * width2 + i + 1] + 2 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 3) * width3 + q + 7] = (5 * Y2[j * width2 + i + 1] + 3 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 4) * width3 + q + 7] = (3 * Y2[j * width2 + i + 1] + 5 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 5) * width3 + q + 7] = (2 * Y2[j * width2 + i + 1] + 6 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 6) * width3 + q + 7] = (1 * Y2[j * width2 + i + 1] + 7 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 7) * width3 + q + 7] = (0 * Y2[j * width2 + i + 1] + 8 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//					  
			//Y3[(p + 7) * width3 + q + 1] = (7 * Y2[(j + 1)* width2 + i] + 1 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 7) * width3 + q + 2] = (6 * Y2[(j + 1)* width2 + i] + 2 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 7) * width3 + q + 3] = (5 * Y2[(j + 1)* width2 + i] + 3 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 7) * width3 + q + 4] = (3 * Y2[(j + 1)* width2 + i] + 5 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 7) * width3 + q + 5] = (2 * Y2[(j + 1)* width2 + i] + 6 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			//Y3[(p + 7) * width3 + q + 6] = (1 * Y2[(j + 1)* width2 + i] + 7 * Y2[(j + 1) * width2 + i + 1]) / 8.0;
			////-------------------------------------------------------------------------------------------------

			//Y3[(p + 1) * width3 + q + 1] = (7 * Y3[(p + 1)* width3 + q] + 1 * Y3[(p + 1) * width3 + q + 7]) / 8.0;
			//Y3[(p + 1) * width3 + q + 2] = (6 * Y3[(p + 1)* width3 + q] + 2 * Y3[(p + 1) * width3 + q + 7]) / 8.0;
			//Y3[(p + 1) * width3 + q + 3] = (5 * Y3[(p + 1)* width3 + q] + 3 * Y3[(p + 1) * width3 + q + 7]) / 8.0;
			//Y3[(p + 1) * width3 + q + 4] = (3 * Y3[(p + 1)* width3 + q] + 5 * Y3[(p + 1) * width3 + q + 7]) / 8.0;
			//Y3[(p + 1) * width3 + q + 5] = (2 * Y3[(p + 1)* width3 + q] + 6 * Y3[(p + 1) * width3 + q + 7]) / 8.0;
			//Y3[(p + 1) * width3 + q + 6] = (1 * Y3[(p + 1)* width3 + q] + 7 * Y3[(p + 1) * width3 + q + 7]) / 8.0;
			//					  
			//Y3[(p + 2) * width3 + q + 1] = (6 * Y3[p * width3 + q + 1] + 2 * Y3[(p + 7) * width3 + q + 1]) / 8.0;
			//Y3[(p + 3) * width3 + q + 1] = (5 * Y3[p * width3 + q + 1] + 3 * Y3[(p + 7) * width3 + q + 1]) / 8.0;
			//Y3[(p + 4) * width3 + q + 1] = (3 * Y3[p * width3 + q + 1] + 5 * Y3[(p + 7) * width3 + q + 1]) / 8.0;
			//Y3[(p + 5) * width3 + q + 1] = (2 * Y3[p * width3 + q + 1] + 6 * Y3[(p + 7) * width3 + q + 1]) / 8.0;
			//Y3[(p + 6) * width3 + q + 1] = (1 * Y3[p * width3 + q + 1] + 7 * Y3[(p + 7) * width3 + q + 1]) / 8.0;
			//					  				   								 				   
			//Y3[(p + 2) * width3 + q + 6] = (6 * Y3[p * width3 + q + 6] + 2 * Y3[(p + 7) * width3 + q + 6]) / 8.0;
			//Y3[(p + 3) * width3 + q + 6] = (5 * Y3[p * width3 + q + 6] + 3 * Y3[(p + 7) * width3 + q + 6]) / 8.0;
			//Y3[(p + 4) * width3 + q + 6] = (3 * Y3[p * width3 + q + 6] + 5 * Y3[(p + 7) * width3 + q + 6]) / 8.0;
			//Y3[(p + 5) * width3 + q + 6] = (2 * Y3[p * width3 + q + 6] + 6 * Y3[(p + 7) * width3 + q + 6]) / 8.0;
			//Y3[(p + 6) * width3 + q + 6] = (1 * Y3[p * width3 + q + 6] + 7 * Y3[(p + 7) * width3 + q + 6]) / 8.0;
			//					  
			//Y3[(p + 6) * width3 + q + 2] = (5 * Y3[(p + 6) * width3 + q] + 3 * Y3[(p + 6) * width3 + q + 7]) / 8.0;
			//Y3[(p + 6) * width3 + q + 3] = (3 * Y3[(p + 6) * width3 + q] + 5 * Y3[(p + 6) * width3 + q + 7]) / 8.0;
			//Y3[(p + 6) * width3 + q + 4] = (3 * Y3[(p + 6) * width3 + q] + 5 * Y3[(p + 6) * width3 + q + 7]) / 8.0;
			//Y3[(p + 6) * width3 + q + 5] = (5 * Y3[(p + 6) * width3 + q] + 3 * Y3[(p + 6) * width3 + q + 7]) / 8.0;
			////-------------------------------------------------------------------------------------------------

			//Y3[(p + 2) * width3 + q + 2] = (5 * Y3[(p + 2) * width3 + q + 1] + 1 * Y3[(p + 2) * width3 + q + 6]) / 6.0;
			//Y3[(p + 2) * width3 + q + 3] = (4 * Y3[(p + 2) * width3 + q + 1] + 2 * Y3[(p + 2) * width3 + q + 6]) / 6.0;
			//Y3[(p + 2) * width3 + q + 4] = (2 * Y3[(p + 2) * width3 + q + 1] + 4 * Y3[(p + 2) * width3 + q + 6]) / 6.0;
			//Y3[(p + 2) * width3 + q + 5] = (1 * Y3[(p + 2) * width3 + q + 1] + 5 * Y3[(p + 2) * width3 + q + 6]) / 6.0;
			//					  									  						 
			//Y3[(p + 3) * width3 + q + 2] = (4 * Y3[(p + 1) * width3 + q + 2] + 2 * Y3[(p + 6) * width3 + q + 2]) / 6.0;
			//Y3[(p + 4) * width3 + q + 2] = (2 * Y3[(p + 1) * width3 + q + 2] + 4 * Y3[(p + 6) * width3 + q + 2]) / 6.0;
			//Y3[(p + 5) * width3 + q + 2] = (1 * Y3[(p + 1) * width3 + q + 2] + 5 * Y3[(p + 6) * width3 + q + 2]) / 6.0;
			//				  										  							 
			//Y3[(p + 3) * width3 + q + 5] = (4 * Y3[(p + 1) * width3 + q + 5] + 2 * Y3[(p + 6) * width3 + q + 5]) / 6.0;
			//Y3[(p + 4) * width3 + q + 5] = (2 * Y3[(p + 1) * width3 + q + 5] + 4 * Y3[(p + 6) * width3 + q + 5]) / 6.0;
			//Y3[(p + 5) * width3 + q + 5] = (1 * Y3[(p + 1) * width3 + q + 5] + 5 * Y3[(p + 6) * width3 + q + 5]) / 6.0;
			//				  										  								 
			//Y3[(p + 5) * width3 + q + 3] = (4 * Y3[(p + 5) * width3 + q + 1] + 2 * Y3[(p + 5) * width3 + q + 6]) / 6.0;
			//Y3[(p + 5) * width3 + q + 4] = (2 * Y3[(p + 5) * width3 + q + 1] + 4 * Y3[(p + 5) * width3 + q + 6]) / 6.0;
			////-------------------------------------------------------------------------------------------------

			//Y3[(p + 3) * width3 + q + 3] = 
			//	(2 * Y3[(p + 3) * width3 + q + 2] + 1 * Y3[(p + 3) * width3 + q + 5] + 2 * Y3[(p + 2) * width3 + q + 3] + 1 * Y3[(p + 5) * width3 + q + 3]) / 6.0;
			//Y3[(p + 3) * width3 + q + 4] = 										  
			//	(1 * Y3[(p + 3) * width3 + q + 2] + 2 * Y3[(p + 3) * width3 + q + 5] + 2 * Y3[(p + 2) * width3 + q + 4] + 1 * Y3[(p + 5) * width3 + q + 4]) / 6.0;
			//Y3[(p + 4) * width3 + q + 3] = 									  				  					 					
			//	(2 * Y3[(p + 4) * width3 + q + 2] + 1 * Y3[(p + 4) * width3 + q + 5] + 1 * Y3[(p + 2) * width3 + q + 3] + 2 * Y3[(p + 5) * width3 + q + 3]) / 6.0;
			//Y3[(p + 4) * width3 + q + 4] = 									  				  									
			//	(1 * Y3[(p + 4) * width3 + q + 2] + 2 * Y3[(p + 4) * width3 + q + 5] + 1 * Y3[(p + 2) * width3 + q + 4] + 2 * Y3[(p + 5) * width3 + q + 4]) / 6.0;
		}
		printf("%d ", j);
	}
	for (int j = 0; j < height - 3; j++)
	{
		Y3[j * width + 511] = Y3[j * width + 508];
		Y3[j * width + 510] = Y3[j * width + 508];
		Y3[j * width + 509] = Y3[j * width + 508];
	}
	for (int i = 0; i < width; i++)
	{
		Y3[511 * width + i] = Y3[508 * width + i];
		Y3[510 * width + i] = Y3[508 * width + i];
		Y3[509 * width + i] = Y3[508 * width + i];
	}

	printf("\n4444mse\n");
	double mse = 0, psnr;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			mse += (double)((Y3[j * width + i] - Y1[j * width + i]) * (Y3[j * width + i] - Y1[j * width + i]));
		}
	}
	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.5lf\nPSNR = %.5lf dB\n", mse, psnr);

	printf("5555¾Æ¿ôÇ²\n");
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y1[j * width + i];
		}
	}

	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++)
		{
			outputImg1[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
			outputImg1[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
			outputImg1[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
		}
	}

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg2[j * stride + 3 * i + 0] = Y3[j * width + i];
			outputImg2[j * stride + 3 * i + 1] = Y3[j * width + i];
			outputImg2[j * stride + 3 * i + 2] = Y3[j * width + i];
		}
	}

	printf("6666Ãâ·Â\n");
	FILE* outputFile = fopen("test3-Y.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	FILE* outputFile1 = fopen("test3-down.bmp", "wb");
	bmpInfo.biWidth = width2;
	bmpInfo.biHeight = height2;
	bmpInfo.biSizeImage = size2;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
	fwrite(outputImg1, sizeof(unsigned char), size2, outputFile1);

	FILE* outputFile2 = fopen("test3-up.bmp", "wb");
	bmpInfo.biWidth = width;
	bmpInfo.biHeight = height;
	bmpInfo.biSizeImage = size;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg2, sizeof(unsigned char), size, outputFile2);


	free(inputImg);
	free(outputImg);
	free(outputImg1);
	free(outputImg2);
	fclose(inputFile);
	fclose(outputFile);
	fclose(outputFile1);
	fclose(outputFile2);


	return 0;
}
