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

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* Y = (unsigned char*)calloc(size, sizeof(unsigned char));
	unsigned char* Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	int* e = (int*)calloc(size, sizeof(int));
	int* d = (int*)calloc(size, sizeof(int));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y[j * width + i] = inputImg[j * stride + 3 * i];
		}
	}

	FILE* text = fopen("Binarization.txt", "w");
	unsigned int* ch = (unsigned int*)calloc(size, sizeof(unsigned int));
	unsigned char* c = (unsigned char*)calloc(size, sizeof(unsigned char) * 3);
	int p = 0;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			p = 128;
			if (i == 0) {
				e[j * width + i] = p - Y[j * width + i] ;
			}
			else {
				d[j * width + i] =  e[j * width + i - 1] / 5;
				if (d[j * width + i] < -3) d[j * width + i]= -4;
				if (d[j * width + i] == -3) d[j * width + i] = -3;
				if (d[j * width + i] == -2) d[j * width + i] = -2;
				if (d[j * width + i] == -1) d[j * width + i] = -1;
				if (d[j * width + i] == 0) d[j * width + i]= 0;
				if (d[j * width + i] == 1) d[j * width + i]= 1;
				if (d[j * width + i] == 2) d[j * width + i]= 2;
				if (d[j * width + i] > 2) d[j * width + i] = 3;
				d[j * width + i] *= 5;
				d[j * width + i] = d[j * width + i] + e[j * width + i - 1];
			}
		}
	}
	
	//for (int j = 0; j < height; j++) {
	//	for (int i = 0; i < width; i++) {
	//		/*ch = e[j * width + i] / 5;*/
	//		if (e[j * width + i] / 5 < -3) c = "000";
	//		if (e[j * width + i] / 5 == -3) c = "001";
	//		if (e[j * width + i] / 5 == -2) c = "010";
	//		if (e[j * width + i] / 5 == -1) c = "011";
	//		if (e[j * width + i] / 5 == 0) c = "100";
	//		if (e[j * width + i] / 5 == 1) c = "101";
	//		if (e[j * width + i] / 5 == 2) c = "110";
	//		if (e[j * width + i] / 5 > 2) c = "111";
	//		fprintf(text, "%s ", c);
	//	}
	//}


	/*char* c = (char*)calloc(size, sizeof(char) * 3);
	FILE* text = fopen("Binarization.txt", "r");
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			fscanf(text, "%s ", c);
			if (strcmp(c, "000") == 0) e[j * width + i] = -4;
			if (strcmp(c, "001") == 0) e[j * width + i] = -3;
			if (strcmp(c, "010") == 0) e[j * width + i] = -2;
			if (strcmp(c, "011") == 0) e[j * width + i] = -1;
			if (strcmp(c, "100") == 0) e[j * width + i] = 0;
			if (strcmp(c, "101") == 0) e[j * width + i] = 1;
			if (strcmp(c, "110") == 0) e[j * width + i] = 2;
			if (strcmp(c, "111") == 0) e[j * width + i] = 3;
			e[j * width + i] *= 5;
		}
	}*/

	/*for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			printf("%d ", e[j * width + i]);
		}
	}*/

	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i == 0) {
				p = 128;
				Y1[j * width + i] = p + e[j * width + i];
			}
			else {
				p = p + e[j * width + i - 1];
				Y1[j * width + i] = p + e[j * width + i];
			}
		}
	}

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			printf("%d ", Y1[j * width + i]);
		}
	}*/
	//double mse = 0, psnr;

	//for (int j = 0; j < height; j++) {
	//	for (int i = 0; i < width; i++) {
	//		mse += (double)((Y1[j * width + i] - Y[j * width + i]) * (Y1[j * width + i] - Y[j * width + i]));
	//	}
	//}
	//mse /= (height * width);
	//psnr = mse != 0 ? 10 * log10(255 * 255 / mse) : 99.99;
	//printf("MSE = %.2f\nPSNR = %.2f dB", mse, psnr);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = e[j * width + i];
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y1[j * width + i];
		}
	}

	FILE* outputFile = fopen("BireconDecY.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}