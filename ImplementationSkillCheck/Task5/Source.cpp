#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task5
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	double angle = 45.0;
	double scale = 1.0;

	cv::Point2f centor = cv::Point2f(img.cols / 2, img.rows / 2);
	cv::Mat affine = cv::getRotationMatrix2D(centor, angle, scale);

	cv::warpAffine(img, img, affine, img.size());

	cv::imshow("Task 5", img);
	cv::waitKey(0);

	return 0;
}