#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	BITMAPFILEHEADER bmpFile1;
	BITMAPINFOHEADER bmpInfo1;
	FILE* inputFile = NULL;
	FILE* inputFile1 = NULL;
	inputFile = fopen("AICenter.bmp", "rb");
	inputFile1 = fopen("AICenterY_CombinedNoise.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	/*printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);*/


	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg_ = NULL;

	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg_ = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg_, sizeof(unsigned char), size, inputFile1);


	double Y, Cb, Cr, R, G, B, I, Diff_Y;
	double mse = 0, psnr;
	double filter[3][3] = { {1.0 / 16, 2.0 / 16, 1.0 / 16}, {2.0 / 16, 4.0 / 16, 2.0 / 16} ,{1.0 / 16, 2.0 / 16, 1.0 / 16} };
	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
			Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];
			R = Y + 1.402 * Cr;
			G = Y - 0.714 * Cr - 0.344 * Cb;
			B = Y + 1.772 * Cb;
			I = (unsigned char)((inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 1] + inputImg[j * stride + 3 * i + 0]) / 3);
			outputImg1[j * stride + 3 * i + 2] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg1[j * stride + 3 * i + 1] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg1[j * stride + 3 * i + 0] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg2[j * stride + 3 * i + 2] = (unsigned char)(I > 255 ? 255 : (I < 0 ? 0 : I));
			outputImg2[j * stride + 3 * i + 1] = (unsigned char)(I > 255 ? 255 : (I < 0 ? 0 : I));
			outputImg2[j * stride + 3 * i + 0] = (unsigned char)(I > 255 ? 255 : (I < 0 ? 0 : I));
			Diff_Y = (Y - I)*(Y - I);
			outputImg[j * stride + 3 * i + 2] = (unsigned char)(Diff_Y > 255 ? 255 : (Diff_Y < 0 ? 0 : Diff_Y));
			outputImg[j * stride + 3 * i + 1] = (unsigned char)(Diff_Y > 255 ? 255 : (Diff_Y < 0 ? 0 : Diff_Y));
			outputImg[j * stride + 3 * i + 0] = (unsigned char)(Diff_Y > 255 ? 255 : (Diff_Y < 0 ? 0 : Diff_Y));
		};
	}*/

	for (int j = 1; j < height - 1; j++)
	{
		for (int i = 1; i < width - 1; i++)
		{
			for (int k = 0; k < 3; k++)
			{
				inputImg_[j * stride + 3 * i + k] = filter[0][0] * inputImg_[(j - 1) * stride + 3 * (i - 1) + 0] +
					filter[0][1] * inputImg_[(j - 1) * stride + 3 * (i)+ 0] +
					filter[0][2] * inputImg_[(j - 1) * stride + 3 * (i + 1) + 0] +
					filter[1][0] * inputImg_[(j)*stride + 3 * (i - 1) + 0] +
					filter[1][1] * inputImg_[(j)*stride + 3 * (i)+ 0] +
					filter[1][2] * inputImg_[(j)*stride + 3 * (i + 1) + 0] +
					filter[2][0] * inputImg_[(j + 1) * stride + 3 * (i - 1) + 0] +
					filter[2][1] * inputImg_[(j + 1) * stride + 3 * (i)+ 0] +
					filter[2][2] * inputImg_[(j + 1) * stride + 3 * (i + 1) + 0];
			}
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			inputImg[j * width + i] = inputImg[j * stride + 3 * i + 0];
			inputImg[j * width + i] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			inputImg_[j * width + i] = inputImg_[j * stride + 3 * i + 0];
			mse += (double)((inputImg_[j * width + i] - inputImg[j * width + i]) * (inputImg_[j * width + i] - inputImg[j * width + i]));

			outputImg[j * stride + 3 * i + 2] = inputImg_[j * stride + 3 * i + 2];
			outputImg[j * stride + 3 * i + 1] = inputImg_[j * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 0] = inputImg_[j * stride + 3 * i + 0];
		}
	}
	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);
	FILE* outputFile = fopen("Output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);


	free(inputImg);
	free(inputImg_);
	free(outputImg);
	fclose(inputFile);
	fclose(inputFile1);
	fclose(outputFile);

	return 0;
}