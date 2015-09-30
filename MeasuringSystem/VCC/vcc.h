#pragma once

#include "../Include/global.h"
#include "../Include/define.h"

#include <stdio.h>
#include <vector>


public ref class VCC{
private:
	short threshold;
	short temp_size_x;
	short temp_size_y;
	unsigned short *vc_template;
	unsigned short *vc_inputimage_assembly;
	unsigned char *vc_inputimage_assembly2;
	unsigned char *vc_inputimage_assembly2_2;
	
	//相関計算のLUT（4画素分（16×16））
	char *VCCCrrMp;

	//unsigned long __fastcall sseCount(unsigned long * buff, unsigned long * pr, unsigned long * pm1, unsigned long * pm2, unsigned long * pm3, unsigned long * pm4, unsigned long * pm5);


public:
	VCC(int thres);
	~VCC();
	int VCC_create_template(unsigned char *image, const int x, const int y);
	int VCC_create_inputimage(unsigned char *image, const int x, const int y);
	//int VCC_create_inputimage_assembly(unsigned char *image, const int x, const int y);
	//int VCC_create_inputimage_assembly2(unsigned char *image, const int x, const int y);
	int VCC_matching(bool rollSeekFlag);

	unsigned short *vc_inputimage;
	short inptimg_size_x;
	short inptimg_size_y;
	cv::Mat VCC_imaging(unsigned short *vcc_image, int x, int y);


	//int VCC_matching_assembly(float *x, float *y, int *crr);
	//int VCC_matching_assembly2(float *x, float *y, int *crr);
	double subpixel_x, subpixel_y;
	double least_x, least_y;
	std::vector<int> *surroundCorrelationData;
	int x,y;
	int correlationPoint;
	
	bool databaseFlag;
	bool subpixelFlag;
	//int databaseSearchThreshold;



	//余分
	//bool kalmanFlag;
	//bool databaseClearFlag;
	//bool databaseAllSearchFlag;
	//bool allSeekFlag;
	//int allSeekThreshold;
	//int targetDB_x;//DB_LTrgt_Row
	//int targetDB_y;//DB_LTrgt_Col
};