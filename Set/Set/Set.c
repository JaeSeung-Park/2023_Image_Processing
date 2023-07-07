#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("AICenterY.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);


	unsigned char* inputImg = NULL, * outputImg = NULL, * outputImg1 = NULL, * outputImg2 = NULL, * outputImg3 = NULL,  *outputImg4,
		*outputImg5;

	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg3 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg4 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg5 = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);

	printf("1111\n");
	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		outputImg[j * stride + 3 * i + 0] = 0;
	//		outputImg[j * stride + 3 * i + 1] = 0;
	//		outputImg[j * stride + 3 * i + 2] = 0;
	//	}
	//}
	printf("2222\n");
	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y4 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y5 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y6 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y7 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	//double pi = 3.1415926535;
	//double degree = pi / 3;
	//int x1, y1, x2, y2;
	//int	x0, y0;
	double filter[3][3] = { {1.0 / 16, 2.0 / 16, 1.0 / 16}, {2.0 / 16, 4.0 / 16, 2.0 / 16} ,{1.0 / 16, 2.0 / 16, 1.0 / 16} };
	double filter1[9] = { 0 };
	double Gx, Gy, G, G1, G2;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 2];
		}
	}
	double tmp = 0;
	/*for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			filter1[0] = Y1[(j - 1) * width + (i - 1)];
			filter1[1] = Y1[(j - 1) * width + i];
			filter1[2] = Y1[(j - 1) * width + (i + 1)];
			filter1[3] = Y1[j * width + (i - 1)];
			filter1[4] = Y1[j * width + i];
			filter1[5] = Y1[j * width + (i + 1)];
			filter1[6] = Y1[(j + 1) * width + (i - 1)];
			filter1[7] = Y1[(j + 1) * width + i];
			filter1[8] = Y1[(j + 1) * width + (i + 1)];

			for (int k = 0; k < 8; k++)
			{
				for (int t = 0; t < 8; t++)
				{
					if (filter1[t] > filter1[t + 1]) {
						filter1[t + 1] = tmp;
						filter1[t + 1] = filter1[t];
						filter1[t] = tmp;
					}
				}
			}
			Y1[j * width + i] = (filter1[3] + filter1[5]) / 2;
		}
	}*/
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			Y1[j * width + i] = ((Y1[(j - 1) * width + i - 1]) + (Y1[(j - 1) * width + i]) + (Y1[(j - 1) * width + i + 1])
				+ (Y1[j * width + i - 1]) + (Y1[j * width + i]) + (Y1[j * width + i + 1])
				+ (Y1[(j + 1) * width + i - 1]) + (Y1[(j + 1) * width + i]) + (Y1[(j + 1) * width + i + 1])) / 9.0;
		}
	}

	printf("3333\n");
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			Gx = (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i]) + 1 * (Y1[j * width + i] - Y1[(j - 1) * width + i + 1])
				+ (-2) * (Y1[j * width + i] - Y1[j * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[j * width + i]) + 2 * (Y1[j * width + i] - Y1[j * width + i + 1])
				+ (-1) * (Y1[j * width + i] - Y1[(j + 1) * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[(j + 1) * width + i]) + 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i + 1]);
			Gx = abs(Gx);

			Gy = (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + (-2) * (Y1[j * width + i] - Y1[(j - 1) * width + i]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i + 1])
				+ 0 * (Y1[j * width + i] - Y1[j * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[j * width + i]) + 0 * (Y1[j * width + i] - Y1[j * width + i + 1])
				+ 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i - 1]) + 2 * (Y1[j * width + i] - Y1[(j + 1) * width + i]) + 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i + 1]);
			Gy = abs(Gy);

			G1 = sqrt(Gx * Gx + Gy * Gy);

			Gx = (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i])
				+ 0 * (Y1[j * width + i] - Y1[j * width + i - 1]) + 1 * (Y1[j * width + i] - Y1[j * width + i]);
			Gx = abs(Gx);

			Gy = 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i])
				+ 1 * (Y1[j * width + i] - Y1[j * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[j * width + i]);
			Gy = abs(Gy);

			G2 = sqrt(Gx * Gx + Gy * Gy);

			G = (G1 + G2) / 2;

			if (G1 >= 200 || G2 >= 40)
				Y6[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
			else
				Y6[j * width + i] = (G1 + G2) / 2;

			if (G1 >= 200 && G2 >= 40)
				Y7[j * width + i] = 255;
			else
				Y7[j * width + i] = (G1 + G2) / 2;

			Y2[j * width + i] = (unsigned char)(G1 > 255 ? 255 : (G1 < 0 ? 0 : G1));
		}
	}

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			Gx = (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i]) + 1 * (Y1[j * width + i] - Y1[(j - 1) * width + i + 1])
				+ (-1) * (Y1[j * width + i] - Y1[j * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[j * width + i]) + 1 * (Y1[j * width + i] - Y1[j * width + i + 1])
				+ (-1) * (Y1[j * width + i] - Y1[(j + 1) * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[(j + 1) * width + i]) + 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i + 1]);
			Gx = abs(Gx);

			Gy = (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i + 1])
				+ 0 * (Y1[j * width + i] - Y1[j * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[j * width + i]) + 0 * (Y1[j * width + i] - Y1[j * width + i + 1])
				+ 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i - 1]) + 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i]) + 1 * (Y1[j * width + i] - Y1[(j + 1) * width + i + 1]);
			Gy = abs(Gy);

			G = sqrt(Gx * Gx + Gy * Gy);
			Y3[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
		}
	}

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			Gx = (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i])
				+ 0 * (Y1[j * width + i] - Y1[j * width + i - 1]) + 1 * (Y1[j * width + i] - Y1[j * width + i]);
			Gx = abs(Gx);

			Gy = 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i])
				+ 1 * (Y1[j * width + i] - Y1[j * width + i - 1]) + 0 * (Y1[j * width + i] - Y1[j * width + i]);
			Gy = abs(Gy);

			G = sqrt(Gx * Gx + Gy * Gy);
			Y4[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
		}
	}

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			Gx = 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i]) + 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i + 1])
				+ (-1) * (Y1[j * width + i] - Y1[j * width + i - 1]) + 4 * (Y1[j * width + i] - Y1[j * width + i]) + (-1) * (Y1[j * width + i] - Y1[j * width + i + 1])
				+ 0 * (Y1[j * width + i] - Y1[(j + 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j + 1) * width + i]) + 0 * (Y1[j * width + i] - Y1[(j + 1) * width + i + 1]);
			Gx = abs(Gx);

			Gy = 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j - 1) * width + i]) + 0 * (Y1[j * width + i] - Y1[(j - 1) * width + i + 1])
				+ (-1) * (Y1[j * width + i] - Y1[j * width + i - 1]) + 4 * (Y1[j * width + i] - Y1[j * width + i]) + (-1) * (Y1[j * width + i] - Y1[j * width + i + 1])
				+ 0 * (Y1[j * width + i] - Y1[(j + 1) * width + i - 1]) + (-1) * (Y1[j * width + i] - Y1[(j + 1) * width + i]) + 0 * (Y1[j * width + i] - Y1[(j + 1) * width + i + 1]);
			Gy = abs(Gy);

			G = sqrt(Gx * Gx + Gy * Gy);
			Y5[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
		}
	}

	/*for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			x1 = i;
			y1 = j;
			x0 = y0 = 256;
			x2 = cos(degree) * (x1 - x0) - sin(degree) * (y1 - y0) + x0;
			y2 = sin(degree) * (x1 - x0) + cos(degree) * (y1 - y0) + y0;
			if ((0 < x2 && x2 < 511) && (0 < y2 && y2 < 511))
				Y2[y2 * width + x2] = Y1[j * width + i];
		}
	}*/
	printf("4444\n");

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];

			outputImg1[j * stride + 3 * i + 0] = Y3[j * width + i];
			outputImg1[j * stride + 3 * i + 1] = Y3[j * width + i];
			outputImg1[j * stride + 3 * i + 2] = Y3[j * width + i];

			outputImg2[j * stride + 3 * i + 0] = Y4[j * width + i];
			outputImg2[j * stride + 3 * i + 1] = Y4[j * width + i];
			outputImg2[j * stride + 3 * i + 2] = Y4[j * width + i];

			outputImg3[j * stride + 3 * i + 0] = Y5[j * width + i];
			outputImg3[j * stride + 3 * i + 1] = Y5[j * width + i];
			outputImg3[j * stride + 3 * i + 2] = Y5[j * width + i];

			outputImg4[j * stride + 3 * i + 0] = Y6[j * width + i];
			outputImg4[j * stride + 3 * i + 1] = Y6[j * width + i];
			outputImg4[j * stride + 3 * i + 2] = Y6[j * width + i];

			outputImg5[j * stride + 3 * i + 0] = Y7[j * width + i];
			outputImg5[j * stride + 3 * i + 1] = Y7[j * width + i];
			outputImg5[j * stride + 3 * i + 2] = Y7[j * width + i];
		}
	}

	printf("5555\n");
	FILE* outputFile = fopen("SobelED.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	FILE* outputFile1 = fopen("Prewitt.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
	fwrite(outputImg1, sizeof(unsigned char), size, outputFile1);

	FILE* outputFile2 = fopen("Robert.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg2, sizeof(unsigned char), size, outputFile2);

	FILE* outputFile3 = fopen("Laplacian.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile3);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile3);
	fwrite(outputImg3, sizeof(unsigned char), size, outputFile3);

	FILE* outputFile4 = fopen("Union.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile4);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile4);
	fwrite(outputImg4, sizeof(unsigned char), size, outputFile4);

	FILE* outputFile5 = fopen("Intersection.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile5);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile5);
	fwrite(outputImg5, sizeof(unsigned char), size, outputFile5);


	free(inputImg);
	free(outputImg);
	free(outputImg1);
	free(outputImg2);
	free(outputImg3);
	free(outputImg4);
	fclose(inputFile);
	fclose(outputFile);
	fclose(outputFile1);
	fclose(outputFile2);
	fclose(outputFile3);
	fclose(outputFile4);

	return 0;
}