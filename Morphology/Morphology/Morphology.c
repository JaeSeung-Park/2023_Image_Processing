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


	unsigned char* inputImg = NULL, * outputImg = NULL, * outputImg1 = NULL, * outputImg2 = NULL, * outputImg3 = NULL, * outputImg4 = NULL;

	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg3 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg4 = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);

	printf("1111\n");
	printf("2222\n");
	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y4 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y5 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y6 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y7 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y8 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	double Gx, Gy, G;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 2];
		}
	}

	printf("3333\n");
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			G = Y1[j * width + i];
			Y2[j * width + i] = (unsigned char)(G >= 170 ? 255 : 0);
		}
	}

	printf("4444\n");
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			int c = 0;
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					if (Y2[(j - (p - 1)) * width + (i - (q - 1))] == 255)
						c++;
				}
			}
			if(c == 9){
				for (int p = 0; p < 3; p++)
				{
					for (int q = 0; q < 3; q++)
					{
						Y3[(j - (p - 1)) * width + (i - (q - 1))] == 0;
					}
				}
				Y3[j * width + i] = 255;
			}
		}
	}

	printf("5555\n");
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			int c = 0;
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					if (Y2[(j - (p - 1)) * width + (i - (q - 1))] == 255)
						c++;
				}
			}
			if (c >= 1) 
				Y4[j * width + i] = 255;
		}
	}

	printf("666\n");
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			int c = 0;
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					if (Y2[(j - (p - 1)) * width + (i - (q - 1))] == 255)
						c++;
				}
			}
			if (c == 9) {
				for (int p = 0; p < 3; p++)
				{
					for (int q = 0; q < 3; q++)
					{
						Y5[(j - (p - 1)) * width + (i - (q - 1))] == 0;
					}
				}
				Y5[j * width + i] = 255;
			}
		}
	}
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			int c = 0;
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					if (Y5[(j - (p - 1)) * width + (i - (q - 1))] == 255)
						c++;
				}
			}
			if (c >= 1)
				Y6[j * width + i] = 255;
		}
	}

	printf("777\n");
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			int c = 0;
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					if (Y2[(j - (p - 1)) * width + (i - (q - 1))] == 255)
						c++;
				}
			}
			if (c >= 1)
				Y7[j * width + i] = 255;
		}
	}
	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			int c = 0;
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					if (Y7[(j - (p - 1)) * width + (i - (q - 1))] == 255)
						c++;
				}
			}
			if (c == 9) {
				for (int p = 0; p < 3; p++)
				{
					for (int q = 0; q < 3; q++)
					{
						Y8[(j - (p - 1)) * width + (i - (q - 1))] == 0;
					}
				}
				Y8[j * width + i] = 255;
			}
		}
	}


	printf("88\n");
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

			outputImg3[j * stride + 3 * i + 0] = Y6[j * width + i];
			outputImg3[j * stride + 3 * i + 1] = Y6[j * width + i];
			outputImg3[j * stride + 3 * i + 2] = Y6[j * width + i];

			outputImg4[j * stride + 3 * i + 0] = Y8[j * width + i];
			outputImg4[j * stride + 3 * i + 1] = Y8[j * width + i];
			outputImg4[j * stride + 3 * i + 2] = Y8[j * width + i];
		}
	}

	printf("5555\n");
	FILE* outputFile = fopen("Edge.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	FILE* outputFile1 = fopen("Erosion.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
	fwrite(outputImg1, sizeof(unsigned char), size, outputFile1);

	FILE* outputFile2 = fopen("Dilation.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg2, sizeof(unsigned char), size, outputFile2);

	FILE* outputFile3 = fopen("Opening.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile3);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile3);
	fwrite(outputImg3, sizeof(unsigned char), size, outputFile3);

	FILE* outputFile4 = fopen("Closeing.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile4);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile4);
	fwrite(outputImg4, sizeof(unsigned char), size, outputFile4);


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