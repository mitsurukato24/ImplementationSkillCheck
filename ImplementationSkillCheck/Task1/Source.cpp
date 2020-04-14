#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task1
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	cv::imshow("Task 1", img);
	cv::waitKey(0);

	return 0;
}