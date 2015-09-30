
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "RevisionTool.h"
#include "MainWindow.h"

using namespace MeasuringSystem;

class Approximation{
public:
	std::vector<cv::Point2d> plot;
	int calculate();

	double a0; //定数項
	double a1; //1次の項
	double a2; //2次の項
};

int Approximation::calculate(){
	cv::Mat  matX = cv::Mat::zeros(3, 3, CV_64FC1);
	cv::Mat  matY = cv::Mat::zeros(3, 1, CV_64FC1);
	cv::Mat  matA = cv::Mat::zeros(3, 1, CV_64FC1);
	for (int i = 0; i < plot.size(); i++){
		matX.at<double>(0, 0) += 1;
		matX.at<double>(0, 1) += plot[i].x;
		matX.at<double>(0, 2) += pow(plot[i].x, 2);
		matX.at<double>(1, 0) += plot[i].x;
		matX.at<double>(1, 1) += pow(plot[i].x, 2);
		matX.at<double>(1, 2) += pow(plot[i].x, 3);
		matX.at<double>(2, 0) += pow(plot[i].x, 2);
		matX.at<double>(2, 1) += pow(plot[i].x, 3);
		matX.at<double>(2, 2) += pow(plot[i].x, 4);
		matY.at<double>(0, 0) += plot[i].y;
		matY.at<double>(1, 0) += plot[i].y*plot[i].x;
		matY.at<double>(2, 0) += plot[i].y*pow(plot[i].x, 2);
	}
	matA = matX.inv()*matY;
	a0 = matA.at<double>(0, 0);
	a1 = matA.at<double>(1, 0);
	a2 = matA.at<double>(2, 0);
	return 0;
}

int getVec(std::string filename, std::vector<double> &vec){
	std::ifstream ifs(filename);
	std::string str;
	while (std::getline(ifs, str) && ifs){
		std::stringstream ss;
		ss << str;
		double d;
		ss >> d;
		vec.push_back(d);
	}
	return 0;
}


System::Void RevisionTool::get_distance_button_Click(System::Object^  sender, System::EventArgs^  e) {

	MainWindow^ mw;
	if (this->Owner != nullptr){
		//親のフォームクラス
		mw = static_cast<MainWindow^>(this->Owner);
		this->measure_vaule_box->Text = mw->OriginalValue->Text;
	}
}
System::Void RevisionTool::getBaseLength(){

	MainWindow^ mw;
	if (this->Owner != nullptr){
		//親のフォームクラス
		mw = static_cast<MainWindow^>(this->Owner);
		this->base_length->Text = String::Format("{0:#0.00}", mw->initParams.baselineLength);
	}
}

System::Void RevisionTool::ApplyOffset(){
	MainWindow^ mw;
	if (this->Owner != nullptr){
		//親のフォームクラス
		mw = static_cast<MainWindow^>(this->Owner);
		mw->linear.a2 = Convert::ToDouble(this->linear_a2->Text);
		mw->linear.a1 = Convert::ToDouble(this->linear_a1->Text);
		mw->linear.a0 = Convert::ToDouble(this->linear_a0->Text);
	}
}
System::Void RevisionTool::timer1_Tick(System::Object^  sender, System::EventArgs^  e){
	if (!base_length_read_flag){
		this->readPrevData();
		//this->getBaseLength();
		this->base_length_read_flag = true;
	}
	this->time_count++;
	linearApprox la;
	std::vector<double> correct;
	std::vector<double> measure;
	for (int i = 0; i < this->true_list->Items->Count; i++){
		std::stringstream ss;
		ss << this->ToStdString(this->true_list->GetItemText(this->true_list->Items[i]));
		double d;
		ss >> d;
		correct.push_back(d);
	}
	for (int i = 0; i < this->measure_list->Items->Count; i++){
		std::stringstream ss;
		ss << this->ToStdString(this->measure_list->GetItemText(this->measure_list->Items[i]));
		double d;
		ss >> d;
		measure.push_back(d);
	}
	if (correct.size() < 2){
		if (this->time_count > 100){
			this->toolStripStatusLabel1->Text = "補正値の算出には２組以上のデータが必要です";
		}
		return;
	}	

	Approximation app;

	std::vector<cv::Point2d> arr;
	for (int i = 0; i < measure.size(); i++){
		cv::Point2d p;
		p.x = correct[i];
		p.y = measure[i] - correct[i];
		arr.push_back(p);
	}
	app.plot = arr;
	app.calculate();

	{
		std::stringstream ss;
		ss << app.a2;
		std::string str = ss.str();
		this->linear_a2->Text = this->ToSystemString(str);
	}
	{
		std::stringstream ss;
		ss << app.a1;
		std::string str = ss.str();
		this->linear_a1->Text = this->ToSystemString(str);
	}
	{
		std::stringstream ss;
		ss << app.a0;
		std::string str = ss.str();
		this->linear_a0->Text = this->ToSystemString(str);
	}
}