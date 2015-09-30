#include "../Include/global.h"
#include <vector>

class LeastSquares{
private:
	std::vector<cv::Point2d> elements;
	cv::Mat X;
	cv::Mat A;
	cv::Mat Y;
	int dimension;
	double newton(std::vector<double> equation, double x);
	std::vector<double> derivation(std::vector<double> equation);
	double equationSolution(std::vector<double> equation, double x);
public:
	LeastSquares(int dimension);
	~LeastSquares();
	int addElements(double x, double y);
	double calucation();
};