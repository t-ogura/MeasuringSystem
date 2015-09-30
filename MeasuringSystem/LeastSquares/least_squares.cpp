#include "least_squares.h"
//#include <stdio.h>

LeastSquares::LeastSquares(int dim){
	this->dimension = dim + 1;
	this->X = cv::Mat_<double>::zeros(this->dimension, this->dimension);
	this->A = cv::Mat_<double>::zeros(this->dimension, 1);
	this->Y = cv::Mat_<double>::zeros(this->dimension, 1);
}

LeastSquares::~LeastSquares(){}

int LeastSquares::addElements(double x, double y){
	cv::Point2d p;
	p.x = x;
	p.y = y;
	this->elements.push_back(p);
	return 0;
}

double LeastSquares::calucation(){
	for (int i = 0; i < this->elements.size(); i++){
		for (int y = 0; y < this->dimension; y++){
			for (int x = 0; x < this->dimension; x++){
				this->X.at<double>(x, y) += pow(this->elements[i].x, x+y);
			}
			this->Y.at<double>(y, 0) += pow(this->elements[i].x, y)*this->elements[i].y;
		}
	}
	this->A = this->X.inv()*this->Y;
	std::vector<double> eqation;
	for (int i = 0; i < this->dimension; i++){
		eqation.push_back(this->A.at<double>(i, 0));
	}
	std::vector<double> delta = this->derivation(eqation);
	double a = 0.0;
	double b = this->newton(delta , a);
	int count = 0;
	while (fabs(a - b) > 1.0e-10 && count++ < 10000){
		a = b;
		b = this->newton(delta, a);
		//std::cout << fabs(a - b) << std::endl;
	}
	return a;
}

std::vector<double> LeastSquares::derivation(std::vector<double> get){
	std::vector<double> ans;
	for (int i = 1; i < get.size(); i++){
		ans.push_back(get[i] * i);
	}
	return ans;
}

double LeastSquares::equationSolution(std::vector<double> eq, double x){
	double ans = 0;
	for (int i = 0; i < eq.size(); i++){
		ans += eq[i] * pow(x, i);
	}
	return ans;
}

double LeastSquares::newton(std::vector<double> equation, double x){
	std::vector<double> numerator = derivation(equation);
	return (x - (this->equationSolution(equation, x) / (this->equationSolution(numerator, x))));
}