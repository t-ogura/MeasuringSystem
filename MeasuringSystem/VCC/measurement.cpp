#include "measurement.h"
#include <winbase.h>
#include <chrono>

Measurement::Measurement(double pSize, double fLength, double bLength,int VCC_Threshold, bool centerCamera, double centerFocal){
	
	this->pixelSize = pSize;
	this->focalLength = fLength;
	this->baselineLength = bLength;

	this->camera_L = new ViCap(CAMERA_ID_LEFT, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
	this->camera_R = new ViCap(CAMERA_ID_RIGHT, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
	
	this->VCC_Th = VCC_Threshold;

	this->vcc_L = gcnew VCC(this->VCC_Th);
	this->vcc_R = new VCC(this->VCC_Th);

	this->leftCameraKalmanInit(VCC_INPUT_IMAGE_SIZE_X / 2, VCC_INPUT_IMAGE_SIZE_Y / 2); //TEST
	this->rightCameraKalmanInit(VCC_INPUT_IMAGE_SIZE_X / 2, VCC_INPUT_IMAGE_SIZE_Y / 2); //TEST

	this->kalmanInitialize(MEASUREMENT_KALMANFILTER_PROCESS_NOISE_COV,MEASUREMENT_KALMANFILTER_MEASUREMENT_NOISE_COV);

	this->trackingLoopFlag = false;
	this->trackingState = "No connect";
	this->trackingThreshold = 170;
	this->trackingHomeFlag = false;
	this->trackingMoveFlag = true;
	this->trackingTiming = false;
	this->outControlFlag = false;
	this->outControlAbs = false;

	this->linear_a2 = 0.0;
	this->linear_a1 = 0.0;
	this->linear_a0 = 0.0;

	leftPoint.x = 0;
	leftPoint.y = 0;
	rightPoint.x = 0;
	rightPoint.y = 0;
	centerPoint.x = 0;
	centerPoint.y = 0;

	cv::Mat templateReadImage = cv::imread(TEMPLATE_IMAGE_PATH, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat templateResizeImage(VCC_TEMPLATE_SIZE + 2, VCC_TEMPLATE_SIZE + 2, CV_8UC1);
	cv::resize(templateReadImage, templateResizeImage, templateResizeImage.size());
	vcc_L->VCC_create_template((unsigned char*)templateResizeImage.data, VCC_TEMPLATE_SIZE+2, VCC_TEMPLATE_SIZE+2);
	vcc_R->VCC_create_template((unsigned char*)templateResizeImage.data, VCC_TEMPLATE_SIZE+2, VCC_TEMPLATE_SIZE+2);
	//cv::imshow("temp",templateResizeImage);
}

Measurement::~Measurement(){}


void Measurement::kalmanInitialize(double processNoisCov, double measurementNoiseCov){
	this->KF = new cv::KalmanFilter(2, 1, 0);
	this->KF_State = new cv::Mat_<float>(2, 1);
	this->KF_ProcessNoise = new cv::Mat(2, 1, CV_32F);
	this->KF_Measurement = new cv::Mat_<float>(1, 1);
	this->KF_Measurement->setTo(cv::Scalar(0));

	this->KF->statePre.at<float>(0) = 1000;
	this->KF->statePre.at<float>(1) = 0;
	this->KF->transitionMatrix = *(cv::Mat_<float>(2, 2) << 1, 0, 0, 1);

	cv::setIdentity(this->KF->measurementMatrix);
	cv::setIdentity(this->KF->processNoiseCov, cv::Scalar::all(processNoisCov));
	cv::setIdentity(this->KF->measurementNoiseCov, cv::Scalar::all(measurementNoiseCov));
	cv::setIdentity(this->KF->errorCovPost, cv::Scalar::all(.1));
}

void Measurement::leftCameraKalmanInit(double get_x, double get_y){

	this->vccL_KF = new cv::KalmanFilter(4, 2, 0);
	this->vccL_state = new cv::Mat_<float>(4, 1); /* (x, y, Vx, Vy) */
	this->vccL_processNoise = new cv::Mat(4, 1, CV_32F);
	this->vccL_measurement = new cv::Mat_<float>(2, 1);
	this->vccL_measurement->setTo(cv::Scalar(0));

	vccL_KF->statePre.at<float>(0) = get_x;
	vccL_KF->statePre.at<float>(1) = get_y;
	vccL_KF->statePre.at<float>(2) = 0;
	vccL_KF->statePre.at<float>(3) = 0;
	vccL_KF->transitionMatrix = *(cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	setIdentity(vccL_KF->measurementMatrix);
	setIdentity(vccL_KF->processNoiseCov, cv::Scalar::all(1e-3));
	setIdentity(vccL_KF->measurementNoiseCov, cv::Scalar::all(1e-1));
	setIdentity(vccL_KF->errorCovPost, cv::Scalar::all(.1));

}

void Measurement::rightCameraKalmanInit(double get_x, double get_y){

	this->vccR_KF = new cv::KalmanFilter(4, 2, 0);
	this->vccR_state = new cv::Mat_<float>(4, 1); /* (x, y, Vx, Vy) */
	this->vccR_processNoise = new cv::Mat(4, 1, CV_32F);
	this->vccR_measurement = new cv::Mat_<float>(2, 1);
	this->vccR_measurement->setTo(cv::Scalar(0));

	vccR_KF->statePre.at<float>(0) = get_x;
	vccR_KF->statePre.at<float>(1) = get_y;
	vccR_KF->statePre.at<float>(2) = 0;
	vccR_KF->statePre.at<float>(3) = 0;
	vccR_KF->transitionMatrix = *(cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	setIdentity(vccR_KF->measurementMatrix);
	setIdentity(vccR_KF->processNoiseCov, cv::Scalar::all(1e-3));
	setIdentity(vccR_KF->measurementNoiseCov, cv::Scalar::all(1e-1));
	setIdentity(vccR_KF->errorCovPost, cv::Scalar::all(.1));

}
//
//void Measurement::tracking(char* com, int baudrate){
//	PTU ptu(com, baudrate);
//	//ptu.turnHome();
//	Sleep(2000);
//	ptu.setSpeed();
//	this->trackingState = "Working";
//	while (this->trackingLoopFlag){
//		if (this->trackingHomeFlag){
//			this->trackingState = "Turn home";
//			ptu.turnHome();
//			Sleep(2000);
//			ptu.setSpeed();
//			this->trackingHomeFlag = false;
//			this->trackingState = "Working";
//		}
//		ptu.getPosition();
//		PanTilt pt, my;
//		my.pan = ptu.panAngle;
//		my.tilt = ptu.tiltAngle;
//		this->mtx.lock();
//		if (!this->centerCameraFlag){
//			pt = this->trackingAngle;
//		}
//		this->platformState = my;
//		this->mtx.unlock();
//		if (this->centerCameraFlag){
//			this->camera_C->getImage();
//			cv::waitKey(1);
//			cv::Mat rectImage(this->camera_C->grayImage, cutArea(centerPoint, VCC_INPUT_IMAGE_SIZE_X, VCC_INPUT_IMAGE_SIZE_Y));
//			this->vcc_C->VCC_create_inputimage(rectImage.data, rectImage.cols, rectImage.rows);
//			cv::waitKey(1);
//			this->vcc_C->VCC_matching(true);
//			centerPoint.x += this->vcc_C->x;
//			centerPoint.y += this->vcc_C->y;
//			cv::waitKey(1);
//			this->angle_C = angleCalculation(vcc_C,centerPoint,this->centerCameraFocalLength);
//		}
//		else{
//			while (!this->trackingTiming)cv::waitKey(1);
//			this->trackingTiming = false;
//		}
//		this->mtx.lock();
//		bool pcFlag = this->outControlFlag;
//		this->mtx.unlock();
//		if (pcFlag){
//			this->mtx.lock();
//			this->trackingMoveFlag = false;
//			bool abs = this->outControlAbs;
//			PanTilt movePT = this->outControlPT;
//			this->mtx.unlock();
//			if (abs){
//				ptu.turn(movePT.pan, movePT.tilt, true);
//				this->mtx.lock();
//				this->outControlAbs = false;
//				this->mtx.unlock();
//			}
//			else{
//				ptu.turn(movePT.pan, movePT.tilt);
//			}
//
//			this->mtx.lock();
//			this->outControlFlag = false;
//			this->mtx.unlock();
//		}
//
//		if (this->trackingMoveFlag){
//			this->trackingState = "Working";
//			if (this->centerCameraFlag){
//				int mC = this->vcc_C->correlationPoint;
//				//std::cout << this->vcc_C->matchingParameters[8] << std::endl;
//				if (mC < this->trackingThreshold){
//					ptu.turn(this->angle_C.pan*0.4, this->angle_C.tilt*0.4);
//					//std::cout << "pan[" << RAD2DEG(this->angle_C.pan) << "]\ttilt[" << RAD2DEG(this->angle_C.tilt) << "]" << std::endl;
//				}
//				else{ ptu.turn(0, 0); }
//			}
//			else{
//				this->mtx.lock();
//				int mL = this->vcc_L->correlationPoint;
//				int mR = this->vcc_R->correlationPoint;
//				this->mtx.unlock();
//				if (mL < this->trackingThreshold && mR < this->trackingThreshold){
//					ptu.turn(pt.pan*0.4, pt.tilt*0.4);
//				}
//				else{ ptu.turn(0, 0); }
//			}
//		}
//		else{
//			this->trackingState = "Stoping";
//		}
//	}
//}

//void Measurement::threadTracking(char* com, int baudrate){
//	this->trackingLoopFlag = true;
//	this->trackingThread = std::thread(std::bind(&Measurement::tracking, this, com, baudrate));
//}
//
//void Measurement::threadTrackingJoin(){
//	if (this->trackingThread.joinable())
//		this->trackingThread.join();
//}

Measurement::PanTilt Measurement::angleCalculation(VCC *vcc,cv::Point p, double f){
	double x = p.x + vcc->subpixel_x;
	double y = p.y + vcc->subpixel_y;
	PanTilt pt;
	pt.pan = atan2(x*this->pixelSize, f);
	pt.tilt = atan2(y*this->pixelSize, f);
	return pt;
}

void Measurement::measure(){
	this->camera_L->getImage();
	this->camera_R->getImage();

	const auto start = std::chrono::system_clock::now();


	if (vcc_L->correlationPoint > this->vcc_L->allSeekThreshold && vcc_R->correlationPoint > this->vcc_R->allSeekThreshold)
	{
		cv::Mat L_mat = this->camera_L->grayImage.clone();
		this->vcc_L->VCC_create_inputimage_assembly2((unsigned char*)L_mat.data, L_mat.cols, L_mat.rows);
		float x_l, y_l;
		int crr_l;

		cv::Mat R_mat = this->camera_R->grayImage.clone();
		this->vcc_R->VCC_create_inputimage_assembly2((unsigned char*)R_mat.data, R_mat.cols, R_mat.rows);
		float x_r, y_r;
		int crr_r;

		using namespace System;
		using namespace System::Threading;
		Thread^ vccThread_L = gcnew Thread(gcnew ParameterizedThreadStart(this->vcc_L,&VCC::VCC_matching_assembly2));
		vccThread_L = std::thread(std::bind(&VCC::VCC_matching_assembly2, this->vcc_L, &x_l, &y_l, &crr_l));
		vccThread_R = std::thread(std::bind(&VCC::VCC_matching_assembly2, this->vcc_R, &x_r, &y_r, &crr_r));
		vccThread_L.join();
		vccThread_R.join();
		leftPoint.x = x_l - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
		leftPoint.y = y_l - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;
		rightPoint.x = x_r - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
		rightPoint.y = y_r - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;

	}
	else if (vcc_L->correlationPoint > this->vcc_L->allSeekThreshold){
		cv::Mat local = this->camera_L->grayImage.clone();
		this->vcc_L->VCC_create_inputimage_assembly2((unsigned char*)local.data, local.cols, local.rows);
		float x, y;
		int crr;
		this->vcc_L->VCC_matching_assembly2(&x, &y, &crr);
		leftPoint.x = x - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
		leftPoint.y = y - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;
	}
	else if (vcc_R->correlationPoint > this->vcc_R->allSeekThreshold){
		cv::Mat local = this->camera_R->grayImage.clone();
		this->vcc_R->VCC_create_inputimage_assembly2((unsigned char*)local.data, local.cols, local.rows);
		float x, y;
		int crr;
		this->vcc_R->VCC_matching_assembly2(&x, &y, &crr);
		rightPoint.x = x - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
		rightPoint.y = y - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;
	}

	cv::Mat rectL(this->camera_L->grayImage, cutArea(leftPoint, VCC_INPUT_IMAGE_SIZE_X, VCC_INPUT_IMAGE_SIZE_Y));
	cv::Mat rectR(this->camera_R->grayImage, cutArea(rightPoint, VCC_INPUT_IMAGE_SIZE_X, VCC_INPUT_IMAGE_SIZE_Y));
	cv::Mat areaL = rectL.clone();
	cv::Mat areaR = rectR.clone();
	this->vcc_L->VCC_create_inputimage((unsigned char*)areaL.data, areaL.cols, areaL.rows);
	this->vcc_R->VCC_create_inputimage((unsigned char*)areaR.data, areaR.cols, areaR.rows);
	this->mtx.lock();
	vccThread_L = std::thread(std::bind(&VCC::VCC_matching, this->vcc_L, true));
	vccThread_R = std::thread(std::bind(&VCC::VCC_matching, this->vcc_R, true));
	vccThread_L.join();
	vccThread_R.join();
	this->mtx.unlock();

	//デバッグ用ここから
	//cv::rectangle(rectR, cv::Rect(vcc_R->x, vcc_R->y, VCC_TEMPLATE_SIZE, VCC_TEMPLATE_SIZE), cv::Scalar(0, 0, 0), -1);
	//cv::imshow("test", rectR);
	//std::cout << "rows:" << rectL.rows << std::endl;
	//this->surroundVCCFlag = true;
	//this->vcc_L->templateMatchingSurround(vcc_L->matchingParameters[2], vcc_L->matchingParameters[3], 1);
	//this->vcc_R->templateMatchingSurround(vcc_R->matchingParameters[2], vcc_R->matchingParameters[3], 1);
	//デバッグ用ここまで
	leftPoint.x = leftPoint.x - VCC_SEEK_AREA_SIZE / 2 + vcc_L->x + VCC_TEMPLATE_HALF_SIZE;
	leftPoint.y = leftPoint.y - VCC_SEEK_AREA_SIZE / 2 + vcc_L->y + VCC_TEMPLATE_HALF_SIZE;
	rightPoint.x = rightPoint.x - VCC_SEEK_AREA_SIZE / 2 + vcc_R->x + VCC_TEMPLATE_HALF_SIZE;
	rightPoint.y = rightPoint.y - VCC_SEEK_AREA_SIZE / 2 + vcc_R->y + VCC_TEMPLATE_HALF_SIZE;


	const auto end = std::chrono::system_clock::now();
	std::cout << "VCC time " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\t";
	std::cout << "left\tx[ " << leftPoint.x;
	std::cout << "]\ty[ " << leftPoint.y;
	std::cout << "]\tcorr[" << vcc_L->correlationPoint;
	std::cout << "]\tright x[ " << rightPoint.x;
	std::cout << "]\ty[ " << rightPoint.y;
	std::cout << "]\tcorr[ " << vcc_R->correlationPoint;
	std::cout << "]";
	//std::cout << "\tsubx[" << vcc_R->subpixel_x;
	//std::cout << "]";
	//std::cout << std::endl;
	//cv::waitKey(0);
	//std::cout << "x:" << rightPoint.x << "\ty:" << rightPoint.y << "\tcrr:" << vcc_R->correlationPoint << std::endl;
	this->angle_L = angleCalculation(vcc_L, leftPoint, this->focalLength);
	this->angle_R = angleCalculation(vcc_R,rightPoint, this->focalLength);
	this->non_offset.original = this->baselineLength / (tan(this->angle_L.pan) - tan(this->angle_R.pan));
	this->non_offset.mid = sqrt(pow(this->non_offset.original, 2)*(pow(tan(angle_L.pan), 2) + pow(tan(angle_R.pan), 2) + 2) / 2 - pow((double)this->baselineLength, 2) / 4);
	this->non_offset.theta = acos(this->non_offset.original / this->non_offset.mid);

	if (this->vcc_L->correlationPoint < this->vcc_L->databaseSearchThreshold && this->vcc_R->correlationPoint < this->vcc_R->databaseSearchThreshold){
		if (prev_distances.size() < 5) prev_distances.push_back(this->distance.original);
		else{
			for (int i = 1; i < prev_distances.size(); i++){
				prev_distances[i - 1] = prev_distances[i];
			}
			prev_distances.pop_back();
			prev_distances.push_back(non_offset.original);
		}
	}
	double prev_dist_kf = this->distance.kf;
	double prev_dist_mid = this->distance.mid;
	double prev_average_distance = 0;
	for (int i = 0; i < prev_distances.size(); i++){
		prev_average_distance += prev_distances[i];
	}
	prev_average_distance /= prev_distances.size();
	double qua = quadratic(this->linear_a2, this->linear_a1 + 1, this->linear_a0 - this->non_offset.original);
	if (qua < 0) this->distance.original = this->non_offset.original;
	else this->distance.original = qua;
	this->distance.mid = sqrt(pow(this->distance.original, 2)*(pow(tan(angle_L.pan), 2) + pow(tan(angle_R.pan), 2) + 2) / 2 - pow((double)this->baselineLength, 2) / 4);
	this->distance.theta = acos(this->distance.original / this->distance.mid);
	if (angle_L.pan + angle_R.pan < 0) this->distance.theta *= -1;
	cv::Mat prediction = this->KF->predict();
	double predictDist = prediction.at<float>(0);
	if (fabs(this->non_offset.mid - prev_average_distance)>500){ (*this->KF_Measurement)(0) = prev_dist_kf;}
	else if (this->vcc_L->correlationPoint > this->vcc_L->databaseSearchThreshold && this->vcc_R->correlationPoint > this->vcc_R->databaseSearchThreshold){ (*this->KF_Measurement)(0) = prev_dist_kf; }
	else (*this->KF_Measurement)(0) = this->distance.mid;
	cv::Mat estimated = this->KF->correct((*this->KF_Measurement));
	this->distance.kf = estimated.at<float>(0);
	PanTilt pt;
	pt.pan = this->distance.theta;
	pt.tilt = (angle_L.tilt + angle_R.tilt) / 2.0;
	this->mtx.lock();
	this->trackingAngle = pt;
	this->mtx.unlock();
	this->trackingTiming = true;
	//std::cout << this->non_offset.original << std::endl;
	cv::imshow("left vcc", vcc_L->VCC_imaging(vcc_L->vc_inputimage, vcc_L->inptimg_size_x - 3, vcc_L->inptimg_size_y));
	cv::imshow("right vcc", vcc_R->VCC_imaging(vcc_R->vc_inputimage, vcc_R->inptimg_size_x - 3, vcc_R->inptimg_size_y));
}


double Measurement::quadratic(double a2, double a1, double a0)
{
	double Re1, Re2, Im1, Im2, D;

	D = a1*a1 - 4.0*a2*a0;

	if (D>0) {
		Re1 = (-a1 + sqrt(D)) / (2.0*a2);
		Re2 = (-a1 - sqrt(D)) / (2.0*a2);
		Im1 = 0.0;
		Im2 = 0.0;
	}
	else if (D == 0) {
		Re1 = -a1 / (2.0*a2);
		Im1 = 0.0;
	}
	else {
		Re1 = Re2 = -a1 / (2.0*a2);
		Im1 = sqrt(-D) / (2.0*a2);
		Im2 = -Im1;
	}

	if (D == 0) {
		return Re1;
	}
	else {
		if (Re1 > 0) return Re1;
		if (Re2 > 0) return Re2;
	}
	return -1;
}

cv::Rect Measurement::cutArea(cv::Point p, int w, int h){
	cv::Rect re;
	re.x = p.x + w / 2 - VCC_SEEK_AREA_SIZE / 2;
	if (re.x < 0){
		re.x = 0;
	}
	else if (re.x > w - VCC_SEEK_AREA_SIZE - 1){
		re.x = w - VCC_SEEK_AREA_SIZE - 1;
	}
	re.y = p.y + h / 2 - VCC_SEEK_AREA_SIZE / 2;
	if (re.y < 0){
		re.y = 0;
	}
	else if (re.y > h - VCC_SEEK_AREA_SIZE - 1){
		re.y = h - VCC_SEEK_AREA_SIZE - 1;
	}
	re.width = VCC_SEEK_AREA_SIZE;
	re.height = VCC_SEEK_AREA_SIZE;
	return re;
}