#include "MainWindow.h"
#pragma comment(lib,"winmm.lib")

using namespace MeasuringSystem;

std::vector<double>prev_distances;

struct PanTilt{
	double pan;
	double tilt;
};

cv::Rect cutArea(cv::Point p, int w, int h){
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

double quadratic(double a2, double a1, double a0)
{
	double Re1, Re2, Im1, Im2, D;
	double Re3;

	D = a1*a1 - 4.0*a2*a0;

	if (D > 0) {
		Re1 = (-a1 + sqrt(D)) / (2.0*a2);
		Re2 = (-a1 - sqrt(D)) / (2.0*a2);
		Re3 = (-a1) / (2.0*a2);
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

System::Void imageShow(cv::Mat inputImage, Graphics ^outputImage){
	Bitmap^ bmp;
	IntPtr ip(inputImage.ptr());// OpenCVの画像バッファをintポインタに変換
	bmp = gcnew Bitmap(inputImage.cols, inputImage.rows, inputImage.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ip);
	outputImage->DrawImage(bmp, 0, 0, inputImage.cols, inputImage.rows);
	delete outputImage;
	delete bmp;
}

PanTilt angleCalculation(VCC ^vcc, cv::Point p, double f, double pixelSize){
	double x = p.x + vcc->subpixel_x;
	double y = p.y + vcc->subpixel_y;
	PanTilt pt;
	pt.pan = atan2(x*pixelSize, f);
	pt.tilt = atan2(y*pixelSize, f);
	return pt;
}


System::Void MainWindow::KalmanFilterParameters::kalmanInitialize(double processNoisCov, double measurementNoiseCov, double dist){
	this->KF = new cv::KalmanFilter(2, 1, 0);
	this->KF_State = new cv::Mat_<float>(2, 1);
	this->KF_ProcessNoise = new cv::Mat(2, 1, CV_32F);
	this->KF_Measurement = new cv::Mat_<float>(1, 1);
	this->KF_Measurement->setTo(cv::Scalar(0));

	this->KF->statePre.at<float>(0) = dist;
	this->KF->statePre.at<float>(1) = 0;
	this->KF->transitionMatrix = *(cv::Mat_<float>(2, 2) << 1, 0, 0, 1);

	cv::setIdentity(this->KF->measurementMatrix);
	cv::setIdentity(this->KF->processNoiseCov, cv::Scalar::all(processNoisCov));
	cv::setIdentity(this->KF->measurementNoiseCov, cv::Scalar::all(measurementNoiseCov));
	cv::setIdentity(this->KF->errorCovPost, cv::Scalar::all(.1));

	this->kalmanInitFinish = true;
}

System::Void MainWindow::mesuring(){
	//UIからデータを取得
	this->readThreshold();

	//画像取得
	this->cameraL->getImage();
	this->cameraR->getImage();
	cv::Mat showImageL = this->cameraL->colorImage.clone();
	cv::Mat showImageR = this->cameraR->colorImage.clone();
	cv::Mat grayImageL = this->cameraL->grayImage.clone();
	cv::Mat grayImageR = this->cameraR->grayImage.clone();

	cv::Rect cutAreaL, cutAreaR;
	//領域抽出
	if (vcc_L->correlationPoint > this->leftThreshold.allSearch){
		this->vcc_L->VCC_create_inputimage((unsigned char*)grayImageL.data, grayImageL.cols, grayImageL.rows);
		cutAreaL = cv::Rect(0, 0, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
		this->vccFinish = false;
		this->vcc_L->subpixelFlag = false;
		this->VCC_background->RunWorkerAsync();
	}
	else{
		cutAreaL = cutArea(*leftPoint, VCC_INPUT_IMAGE_SIZE_X, VCC_INPUT_IMAGE_SIZE_Y);
		cv::Mat rectL(grayImageL, cutAreaL);
		cv::Mat areaL = rectL.clone();
		this->vcc_L->VCC_create_inputimage((unsigned char*)areaL.data, areaL.cols, areaL.rows);
		this->vccFinish = false;
		this->vcc_L->subpixelFlag = true;
		this->VCC_background->RunWorkerAsync();
	}
	if (vcc_R->correlationPoint > this->rightThreshold.allSearch){
		this->vcc_R->VCC_create_inputimage((unsigned char*)grayImageR.data, grayImageR.cols, grayImageR.rows);
		cutAreaR = cv::Rect(0, 0, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
		this->vcc_R->subpixelFlag = false;
		this->vcc_R->VCC_matching(false);
	}
	else{
		cutAreaR = cutArea(*rightPoint, VCC_INPUT_IMAGE_SIZE_X, VCC_INPUT_IMAGE_SIZE_Y);
		cv::Mat rectR(grayImageR, cutAreaR);
		cv::Mat areaR = rectR.clone();
		this->vcc_R->VCC_create_inputimage((unsigned char*)areaR.data, areaR.cols, areaR.rows);
		this->vcc_R->subpixelFlag = true;
		this->vcc_R->VCC_matching(true);
	}

	//テンプレートマッチング
	while (1){
		try{
			mtx->WaitOne();
			if (this->vccFinish) break;
		}
		finally{
			mtx->ReleaseMutex();
		}
	}

	//検出位置更新
	leftPoint->x = cutAreaL.x + vcc_L->x - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
	leftPoint->y = cutAreaL.y + vcc_L->y - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;
	rightPoint->x = cutAreaR.x + vcc_R->x - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
	rightPoint->y = cutAreaR.y + vcc_R->y - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;

	//距離計測
	PanTilt angle_L = angleCalculation(vcc_L, *leftPoint, initParams.leftCamera.focalLength, initParams.pixelSize);
	PanTilt angle_R = angleCalculation(vcc_R, *rightPoint, initParams.rightCamera.focalLength, this->initParams.pixelSize);
	double original = this->initParams.baselineLength / (tan(angle_L.pan) - tan(angle_R.pan));
	double mid = sqrt(pow(original, 2)*(pow(tan(angle_L.pan), 2) + pow(tan(angle_R.pan), 2) + 2) / 2 - pow((double)this->initParams.baselineLength, 2) / 4);
	double theta = acos(original / mid);
	this->distances.original = mid;

	//計測値補正
	if (this->vcc_L->correlationPoint < this->leftThreshold.reliability && this->vcc_R->correlationPoint < this->rightThreshold.reliability){
		if (prev_distances.size() < 5){
			prev_distances.push_back(mid);
		}
		else{
			for (int i = 1; i < prev_distances.size(); i++){
				prev_distances[i - 1] = prev_distances[i];
			}
			prev_distances.pop_back();
			prev_distances.push_back(mid);
		}
	}
	double prev_dist_kf = this->distances.kf;
	double prev_dist_revised = this->distances.revised;
	double prev_average_distance = 0;
	for (int i = 0; i < prev_distances.size(); i++){
		prev_average_distance += prev_distances[i];
	}
	prev_average_distance /= prev_distances.size();
	double qua = quadratic(this->linear.a2, this->linear.a1 + 1, this->linear.a0 - mid);
	if (qua < 0) this->distances.revised = mid;
	else this->distances.revised = qua;
	if (angle_L.pan + angle_R.pan < 0) theta *= -1;

	if (!this->distanceKalman.kalmanInitFinish){
		this->distanceKalman.kalmanInitialize(this->initParams.measurementKalmanfilter.processNoiseCov, this->initParams.measurementKalmanfilter.measurementNoiseCov,this->distances.revised);
	}

	cv::Mat prediction = this->distanceKalman.KF->predict();
	double predictDist = prediction.at<float>(0);
	if (fabs(mid - prev_average_distance)>500){ (*this->distanceKalman.KF_Measurement)(0) = prev_dist_kf; }
	else if (this->vcc_L->correlationPoint > this->leftThreshold.reliability && this->vcc_R->correlationPoint > this->rightThreshold.reliability){ (*this->distanceKalman.KF_Measurement)(0) = prev_dist_kf; }
	else (*this->distanceKalman.KF_Measurement)(0) = this->distances.revised;
	cv::Mat estimated = this->distanceKalman.KF->correct((*this->distanceKalman.KF_Measurement));
	this->distances.kf = estimated.at<float>(0);


	if (this->distances.kf != this->distances.kf){
		this->distanceKalman.kalmanInitialize(this->initParams.measurementKalmanfilter.processNoiseCov, this->initParams.measurementKalmanfilter.measurementNoiseCov, this->distances.revised);
		std::cout << "kalman initialize" << std::endl;
	}

	//表示関係
	this->OriginalValue->Text = String::Format("{0:#0.00}", mid);
	this->RevisedValue->Text = String::Format("{0:#0.00}", this->distances.revised);
	this->KalmanValue->Text = String::Format("{0:#0.00}", this->distances.kf);

	//描画関係
	cv::rectangle(showImageL, cutAreaL, cv::Scalar(0, 0, 255), 4);
	cv::rectangle(showImageL,
		cv::Rect(cutAreaL.x + vcc_L->x,
		cutAreaL.y + vcc_L->y,
		VCC_TEMPLATE_SIZE, VCC_TEMPLATE_SIZE), cv::Scalar(0, 255, 255), 4);
	std::stringstream ssL;
	ssL << vcc_L->correlationPoint;
	int printy = cutAreaL.y + vcc_L->y + VCC_SEEK_AREA_SIZE;
	if (cutAreaL.y + vcc_L->y + VCC_SEEK_AREA_SIZE / 2 > VCC_INPUT_IMAGE_SIZE_Y / 2) printy -= (VCC_SEEK_AREA_SIZE + 55);
	cv::putText(showImageL, ssL.str(), cv::Point(cutAreaL.x, printy), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);

	cv::rectangle(showImageR, cutAreaR, cv::Scalar(0, 0, 255), 4);
	cv::rectangle(showImageR,
		cv::Rect(cutAreaR.x + vcc_R->x,
		cutAreaR.y + vcc_R->y,
		VCC_TEMPLATE_SIZE, VCC_TEMPLATE_SIZE), cv::Scalar(0, 255, 255), 4);
	std::stringstream ssR;
	ssR << vcc_R->correlationPoint;
	printy = cutAreaR.y + vcc_R->y + VCC_SEEK_AREA_SIZE;
	if (cutAreaR.y + vcc_R->y + VCC_SEEK_AREA_SIZE / 2 > VCC_INPUT_IMAGE_SIZE_Y / 2) printy -= (VCC_SEEK_AREA_SIZE + 55);
	cv::putText(showImageR, ssR.str(), cv::Point(cutAreaR.x, printy), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);

	cv::resize(showImageL, showImageL, cv::Size(SHOW_IMAGE_WIDTH, SHOW_IMAGE_HEIGHT));
	cv::resize(showImageR, showImageR, cv::Size(SHOW_IMAGE_WIDTH, SHOW_IMAGE_HEIGHT));

	//表示画像変換
	imageShow(showImageL, this->leftImageBox->CreateGraphics());
	imageShow(showImageR, this->rightImageBox->CreateGraphics());
}

System::Void MainWindow::VCC_Matching(VCC ^vcc){
	vcc->VCC_matching(true);
}

System::Void MainWindow::viInit(){
	this->cameraL = new ViCap(initParams.leftCamera.id, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
	this->cameraR = new ViCap(initParams.rightCamera.id, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
	this->vcc_L = gcnew VCC(VCC_VECTOR_CODE_THRESHOLD);
	this->vcc_R = gcnew VCC(VCC_VECTOR_CODE_THRESHOLD);
	cv::Mat templateReadImage = cv::imread(TEMPLATE_IMAGE_PATH, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat templateResizeImage(VCC_TEMPLATE_SIZE + 2, VCC_TEMPLATE_SIZE + 2, CV_8UC1);
	cv::resize(templateReadImage, templateResizeImage, templateResizeImage.size());
	vcc_L->VCC_create_template((unsigned char*)templateResizeImage.data, VCC_TEMPLATE_SIZE + 2, VCC_TEMPLATE_SIZE + 2);
	vcc_R->VCC_create_template((unsigned char*)templateResizeImage.data, VCC_TEMPLATE_SIZE + 2, VCC_TEMPLATE_SIZE + 2);

	leftPoint = new cv::Point(0, 0);
	rightPoint = new cv::Point(0, 0);
}

System::Void MainWindow::VCC_background_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e){

	if (vcc_L->correlationPoint > this->leftThreshold.allSearch){
		this->vcc_L->VCC_matching(false);
	}
	else{
		this->vcc_L->VCC_matching(true);
	}
	try{
		mtx->WaitOne();
		this->vccFinish = true;
	}
	finally{
		mtx->ReleaseMutex();
	}
}
System::Void MainWindow::VCC_background_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e)
{
}

double MainWindow::KalmanFilterParameters::angleKalman(double platform, double ditected, int correlation, int threshold){
	double angle = platform + ditected;

	if (correlation < threshold){
		for (int i = 1; i < 5; i++){
			prev_angles[i - 1] = prev_angles[i];
		}
		prev_angles[4] = angle;
	}
	double prev_angle_kf = prev_angle;
	double prev_average_angle = 0;
	for (int i = 0; i < 5; i++){
		prev_average_angle += prev_angles[i];
	}
	prev_average_angle /= 5;

	cv::Mat prediction = this->KF->predict();
	double predictDist = prediction.at<float>(0);
	if (fabs(angle - prev_average_angle) > 10.0){ (*this->KF_Measurement)(0) = prev_angle_kf; }
	else if (correlation > threshold - 20){ (*this->KF_Measurement)(0) = prev_angle_kf; }
	else (*this->KF_Measurement)(0) = angle;
	cv::Mat estimated = this->KF->correct((*this->KF_Measurement));
	prev_angle = estimated.at<float>(0);
	return prev_angle;
}

System::Void MainWindow::Tracking_background_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e){



	this->cameraC = new ViCap(trackingParameters.centerCameraID, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
	this->vcc_C = gcnew VCC(VCC_VECTOR_CODE_THRESHOLD);
	cv::Mat templateReadImage = cv::imread(TEMPLATE_IMAGE_PATH, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat templateResizeImage(VCC_TEMPLATE_SIZE + 2, VCC_TEMPLATE_SIZE + 2, CV_8UC1);
	cv::resize(templateReadImage, templateResizeImage, templateResizeImage.size());
	vcc_C->VCC_create_template((unsigned char*)templateResizeImage.data, VCC_TEMPLATE_SIZE + 2, VCC_TEMPLATE_SIZE + 2);
	centerPoint = new cv::Point(0, 0);


	while (1){

		//画像取得
		this->cameraC->getImage();
		cv::Mat showImageC = this->cameraC->colorImage.clone();
		cv::Mat grayImageC = this->cameraC->grayImage.clone();

		cv::Rect cutAreaC;
		//領域抽出
		if (vcc_C->correlationPoint > this->centerThreshold.allSearch){
			this->vcc_C->VCC_create_inputimage((unsigned char*)grayImageC.data, grayImageC.cols, grayImageC.rows);
			cutAreaC = cv::Rect(0, 0, CAP_IMAGE_WIDTH, CAP_IMAGE_HEIGHT);
			this->vcc_C->VCC_matching(false);
		}
		else{
			cutAreaC = cutArea(*centerPoint, VCC_INPUT_IMAGE_SIZE_X, VCC_INPUT_IMAGE_SIZE_Y);
			cv::Mat rectC(grayImageC, cutAreaC);
			cv::Mat areaC = rectC.clone();
			this->vcc_C->VCC_create_inputimage((unsigned char*)areaC.data, areaC.cols, areaC.rows);
			this->vcc_C->VCC_matching(true);
		}

		//検出位置更新
		centerPoint->x = cutAreaC.x + vcc_C->x - VCC_INPUT_IMAGE_SIZE_X / 2 + VCC_TEMPLATE_HALF_SIZE;
		centerPoint->y = cutAreaC.y + vcc_C->y - VCC_INPUT_IMAGE_SIZE_Y / 2 + VCC_TEMPLATE_HALF_SIZE;


		//描画関係
		cv::rectangle(showImageC, cutAreaC, cv::Scalar(0, 0, 255), 4);
		cv::rectangle(showImageC,
			cv::Rect(cutAreaC.x + vcc_C->x,
			cutAreaC.y + vcc_C->y,
			VCC_TEMPLATE_SIZE, VCC_TEMPLATE_SIZE), cv::Scalar(0, 255, 255), 4);
		std::stringstream ss;
		ss << vcc_C->correlationPoint;
		int printy = cutAreaC.y + vcc_C->y + VCC_SEEK_AREA_SIZE;
		if (cutAreaC.y + vcc_C->y + VCC_SEEK_AREA_SIZE / 2 > VCC_INPUT_IMAGE_SIZE_Y / 2) printy -= (VCC_SEEK_AREA_SIZE + 55);
		cv::putText(showImageC, ss.str(), cv::Point(cutAreaC.x, printy), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);

		cv::resize(showImageC, showImageC, cv::Size(SHOW_IMAGE_WIDTH, SHOW_IMAGE_HEIGHT));

		//表示画像変換
		Graphics ^g = this->centerImageBox->CreateGraphics();
		Bitmap^ bmp;
		IntPtr ip(showImageC.ptr());// OpenCVの画像バッファをintポインタに変換
		bmp = gcnew Bitmap(showImageC.cols, showImageC.rows, showImageC.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ip);
		g->DrawImage(bmp, 0, 0, showImageC.cols, showImageC.rows);
		delete g;
		delete bmp;


		PanTilt angle = angleCalculation(vcc_C, *centerPoint, this->trackingParameters.centerCameraFocalLength, this->trackingParameters.centerCameraPixelSize);


		//Pan-Tilter
		if (this->PanTilterEnableCheck->Checked){
			try{
				this->tracking_mtx->WaitOne();

				if (ptu == NULL){
					std::stringstream ss;
					ss << "\\\\.\\COM";
					ss << this->trackingParameters.COMPortNum;
					char *cstr = new char[ss.str().length() + 1];
					strcpy(cstr, ss.str().c_str());
					ptu = new PTU(cstr, 9600);
					ptu->turnHome();
					Sleep(2000);
					ptu->setSpeed();
				}


				//Pan-Tilter角度取得
				ptu->getPosition();

				double pan = panAngleKalman.angleKalman(ptu->panAngle, angle.pan, vcc_C->correlationPoint, centerThreshold.reliability);
				double tilt = tiltAngleKalman.angleKalman(ptu->tiltAngle, angle.tilt, vcc_C->correlationPoint, centerThreshold.reliability);

				//角度伝達
				this->targetAngle.pan = pan;
				this->targetAngle.tilt = tilt;

				if (this->ManualControlFlag){
				}
				else if(vcc_C->correlationPoint < centerThreshold.tracking){
						ptu->turn(angle.pan*0.1, angle.tilt*0.1);
				}
				else{
					ptu->turn(0, 0);
				}
			}
			finally{
				this->tracking_mtx->ReleaseMutex();
			}
		}

	}
}


System::Void MainWindow::readInitFile(std::string filename){
	std::ifstream ifs(filename);
	if (ifs.fail()){
		std::cerr << "File open error [" << filename << "]" << std::endl;
		std::cin.get();
	}

	//初期化
	/* Camera ID */
	initParams.leftCamera.id = 0;
	initParams.rightCamera.id = 1;
	initParams.centerCamera.id = 2;
	/* Camera focal length */
	initParams.leftCamera.focalLength = 16.0;
	initParams.rightCamera.focalLength = 16.0;
	initParams.centerCamera.focalLength = 8.0;
	/* Measurement parameters */
	initParams.baselineLength = 300;
	initParams.pixelSize = 0.006;
	/* Kalman filiter parameters */
	initParams.measurementKalmanfilter.processNoiseCov = 1e-3;
	initParams.measurementKalmanfilter.measurementNoiseCov = 5e-1;
	initParams.angleKalmanfilter.processNoiseCov = 1e-2;
	initParams.angleKalmanfilter.measurementNoiseCov = 5e-1;
	/* Camera platform COM number */
	initParams.COMPortNum = 22;
	/* Data transmit info */
	initParams.serverDirectry = "\\\\rg-laptop-06\\share";
	initParams.clientID = 0;
	initParams.automaticallyTransmit = false;


	std::string str;
	while (ifs&&std::getline(ifs, str)){
		if (str.size() > 1){
			if (str.c_str()[0] == '%'){
				continue;
			}
		}
		std::string token1, token2;
		std::istringstream stream(str);
		std::getline(stream, token1, ' ');
		std::getline(stream, token2);
		std::stringstream ss;
		ss << token2;
		int intToken;
		double doubleToken;
		if (token1 == "LEFT_CAMERA_ID"){
			ss >> intToken;
			initParams.leftCamera.id = intToken;
		}
		else if (token1 == "RIGHT_CAMERA_ID"){
			ss >> intToken;
			initParams.rightCamera.id = intToken;
		}
		else if (token1 == "CENTER_CAMERA_ID"){
			ss >> intToken;
			initParams.centerCamera.id = intToken;
		}
		else if (token1 == "LEFT_CAMERA_FOCAL_LENGTH"){
			ss >> doubleToken;
			initParams.leftCamera.focalLength = doubleToken;
		}
		else if (token1 == "RIGHT_CAMERA_FOCAL_LENGTH"){
			ss >> doubleToken;
			initParams.rightCamera.focalLength = doubleToken;
		}
		else if (token1 == "CENTER_CAMERA_FOCAL_LENGTH"){
			ss >> doubleToken;
			initParams.centerCamera.focalLength = doubleToken;
		}
		else if (token1 == "BASELINE_LENGTH"){
			ss >> doubleToken;
			initParams.baselineLength = doubleToken;
		}
		else if (token1 == "PIXEL_SIZE"){
			ss >> doubleToken;
			initParams.pixelSize = doubleToken;
		}
		else if (token1 == "MEASUREMENT_KALMANFILTER_PROCESS_NOISE_COV"){
			ss >> doubleToken;
			initParams.measurementKalmanfilter.processNoiseCov = doubleToken;
		}
		else if (token1 == "MEASUREMENT_KALMANFILTER_MEASUREMENT_NOISE_COV"){
			ss >> doubleToken;
			initParams.measurementKalmanfilter.measurementNoiseCov = doubleToken;
		}
		else if (token1 == "ANGLE_KALMANFILTER__PROCESS_NOISE_COV"){
			ss >> doubleToken;
			initParams.angleKalmanfilter.processNoiseCov = doubleToken;
		}
		else if (token1 == "ANGLE_KALMANFILTER__MESUREMENT_NOISE_COV"){
			ss >> doubleToken;
			initParams.angleKalmanfilter.measurementNoiseCov = doubleToken;
		}
		else if (token1 == "COM_PORT_NUMBER"){
			ss >> intToken;
			initParams.COMPortNum = intToken;
		}
		else if (token1 == "SERVER_DIRECTRY"){
			initParams.serverDirectry = ToSystemString(ss.str());
		}
	}

	this->trackingParameters.centerCameraID = this->initParams.centerCamera.id;
	this->trackingParameters.centerCameraFocalLength = this->initParams.centerCamera.focalLength;
	this->trackingParameters.centerCameraPixelSize = this->initParams.pixelSize;
	this->trackingParameters.COMPortNum = this->initParams.COMPortNum;
}

System::Void MainWindow::initialize(){
	this->mtx = gcnew Threading::Mutex;
	this->tracking_mtx = gcnew Threading::Mutex;
	this->linear.a0 = 0.0;
	this->linear.a1 = 0.0;
	this->linear.a2 = 0.0;
	this->panAngleKalman.kalmanInitialize(this->initParams.angleKalmanfilter.processNoiseCov, this->initParams.angleKalmanfilter.measurementNoiseCov, 0.0);
	this->tiltAngleKalman.kalmanInitialize(this->initParams.angleKalmanfilter.processNoiseCov, this->initParams.angleKalmanfilter.measurementNoiseCov, 0.0);
	this->ManualControlFlag = false;
}

System::Void MainWindow::readThreshold(){
	this->leftThreshold.allSearch = Convert::ToInt32(this->AllSearchBox->Text);
	this->leftThreshold.tracking = Convert::ToInt32(this->TrackingThresholdBox->Text);
	this->leftThreshold.reliability = Convert::ToInt32(this->ReliabilityBox->Text);
	this->rightThreshold.allSearch = Convert::ToInt32(this->AllSearchBox->Text);
	this->rightThreshold.tracking = Convert::ToInt32(this->TrackingThresholdBox->Text);
	this->rightThreshold.reliability = Convert::ToInt32(this->ReliabilityBox->Text);
}