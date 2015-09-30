#pragma once

#include "../Include/global.h"
#include "videoInput.h"
#pragma comment (lib,"VideoInput/videoInput.lib")

class ViCap{
private:
	int ID;
	videoInput *VI;
	unsigned char* buffer;
	int width;
	int height;
public:
	ViCap(int cameraID, int width, int height);
	~ViCap();
	cv::Mat cameraImage;
	cv::Mat grayImage;
	cv::Mat colorImage;

	void getImage();
};