#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("test.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width2 = bmpInfo.biWidth;
	int height2 = bmpInfo.biHeight;
	int size2 = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	//printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width2, stride2, height2, size2, bitCnt);

	int ratio = 2;
	int width = bmpInfo.biWidth << ratio;
	int height = bmpInfo.biHeight << ratio;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	int size = stride * height;
	//printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);


	unsigned char* inputImg = NULL, * outputImg = NULL;

	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);

	//unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));
	unsigned char* Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));


	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++)
		{
			Y2[j * width2 + i] = inputImg[j * stride2 + 3 * i];
		}
	}


	//업샘플링: 다운샘플링된 이미지의 픽셀 1칸의 크기를 16칸으로 늘려야하므로 
	//다음과 같은 식으로 처리해 주었다.
	//						...
	//						ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ
	//	ex)					ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ
	// 	...					ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ 
	//	사 아 자			라 ㅇ ㅇ ㅇ 마 ㅇ ㅇ ㅇ 바 ㅇ ㅇ ㅇ
	//	라 마 바     --->	ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ
	//	가 나 다	...		ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ
	//						ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ ㅇ ㅡ ㅡ ㅡ	
	//						가 ㅇ ㅇ ㅇ 나 ㅇ ㅇ ㅇ 다 ㅇ ㅇ ㅇ ...
	//		
	//	1. 다운샘플된 이미지의 근접한 4칸의 픽셀(가 나, 라 마)로 선형 보간법을 통해 1픽셀이 16픽셀로 늘어났을때의
	//	태두리를 가중치 4를 거리에 따라 적절히 분배하여(3:1, 2:2, 1:3) 픽셀값을 구함 
	//	(위 예시에서 ㅇ로 나타낸 부분)
	//
	//	   라 ㅇ ㅇ ㅇ 마
	// 	   ㅇ ㅡ ㅡ ㅡ ㅇ
	// 	   ㅇ ㅡ ㅡ ㅡ ㅇ
	// 	   ㅇ ㅡ ㅡ ㅡ ㅇ
	// 	   가 ㅇ ㅇ ㅇ 나
	//
	//	2. ㅡ로 나타낸 부분은 양선형 보간법을 통해 x축과 y축 각각 가중치 4를 두고 거리에 따라 적절히 분배하여(3:1, 2:2, 1:3)
	//  픽셀값을 구함
	//


	int p, q;
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

			//Y3[(p + 2) * width + q + 2] =
			//	(2 * Y3[(p + 2) * width + q + 1] + 2 * Y3[(p + 2) * width + q + 3]
			//		+ 2 * Y3[(p + 1) * width + q + 2] + 2 * Y3[(p + 3) * width + q + 2]) / 8.0;

		}
	}

	//바운더리 처리: 보간법으로 처리가 안된 픽셀들은 값을 알고있는 제일 위쪽과 오른쪽의 픽셀과 같은 값으로 처리해줌(Nearest Neighbor)
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



	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = Y3[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y3[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y3[j * width + i];
		}
	}

	FILE* outputFile = fopen("19011779.bmp", "wb");
	bmpInfo.biWidth = width;
	bmpInfo.biHeight = height;
	bmpInfo.biSizeImage = size;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}