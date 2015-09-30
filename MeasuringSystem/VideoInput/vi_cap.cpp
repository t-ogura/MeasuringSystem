#include "vi_cap.h"

ViCap::ViCap(int id, int w, int h){
	VI = new videoInput;
	this->ID = id;
	VI->setupDevice(this->ID, w, h);
	this->width = VI->getWidth(this->ID);
	this->height = VI->getHeight(this->ID);
	buffer = new unsigned char[VI->getSize(this->ID)];
}

ViCap::~ViCap(){}

void ViCap::getImage(){
	VI->getPixels(this->ID, buffer, false, true);
	this->cameraImage = cv::Mat(this->height, this->width, CV_8UC3, buffer, cv::Mat::AUTO_STEP);
	if (this->cameraImage.channels() > 1){
		cv::cvtColor(cameraImage, grayImage, CV_BGR2GRAY);
		this->colorImage = this->cameraImage.clone();
	}
	else{
		this->grayImage = this->cameraImage.clone();
		cv::cvtColor(cameraImage, colorImage, CV_GRAY2BGR);
	}
}