#ifndef _MEASUREMENT_H_
#define _MEASUREMENT_H_

#include "../VideoInput/vi_cap.h"
#include "vcc.h"
#include "../Include/define.h"

class Measurement{
public:
	Measurement(double imagingDevicePixelSize, double lensFocalLength, double stereoBaselineLength, int VCC_Threshold = 12, bool centerCamera = false, double senterCameraFocalLength = 8.);
	~Measurement();

	std::string platform_comnumber;
	
	void tracking(char* comPortNumber, int baudrate);
	void threadTracking(char* comPortNumber, int baudrate);
	void measure();
	void threadTrackingJoin();

	double pixelSize;
	double focalLength;
	double baselineLength;

	int VCC_Th;
	bool centerCameraFlag;
	double centerCameraFocalLength;

	std::string LEFT_OR_RIGHT;

	ViCap *camera_L;
	ViCap *camera_R;
	ViCap *camera_C;

	VCC ^vcc_L;
	VCC ^vcc_R;
	VCC ^vcc_C;

	//補正値
	double linear_a2;
	double linear_a1;
	double linear_a0;
	
	bool trackingTiming;
	bool trackingHomeFlag;
	bool trackingMoveFlag;
	bool outControlFlag;
	bool outControlAbs;

	cv::Point leftPoint;
	cv::Point rightPoint;
	cv::Point centerPoint;


	int trackingThreshold;
	std::string trackingState;
	bool trackingLoopFlag;
	struct Distances{
		double original;
		double mid;
		double theta;
		double kf;
	};
	Distances distance;
	Distances non_offset;

	struct PanTilt{
		double pan;
		double tilt;
	};
	PanTilt platformState;
	PanTilt trackingAngle;
	PanTilt angle_L;
	PanTilt angle_R;
	PanTilt angle_C;

	PanTilt outControlPT;

	PanTilt angleCalculation(VCC *vcc,cv::Point point, double cameraFocalLength);

	std::vector<double> prev_distances;

	cv::Rect cutArea(cv::Point point, int image_width, int image_height);

	cv::KalmanFilter *KF;
	cv::Mat_<float> *KF_State;
	cv::Mat *KF_ProcessNoise;
	cv::Mat_<float> *KF_Measurement;


	cv::KalmanFilter *vccL_KF;
	cv::Mat_<float> *vccL_state; /* (x, y, Vx, Vy) */
	cv::Mat *vccL_processNoise;
	cv::Mat_<float> *vccL_measurement;

	cv::KalmanFilter *vccR_KF;
	cv::Mat_<float> *vccR_state; /* (x, y, Vx, Vy) */
	cv::Mat *vccR_processNoise;
	cv::Mat_<float> *vccR_measurement;

	//VCCデバッグ用
	bool surroundVCCFlag;

private:


	void kalmanInitialize(double processNoisCov, double measurementNoiseCov);
	void leftCameraKalmanInit(double get_x, double get_y);
	void rightCameraKalmanInit(double get_x, double get_y);
	double quadratic(double a2, double a1, double a0);


	//std::thread trackingThread;

	//std::thread vccThread_L;
	//std::thread vccThread_R;
};

#endif