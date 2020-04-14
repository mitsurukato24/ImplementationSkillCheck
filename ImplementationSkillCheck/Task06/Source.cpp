#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task6
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);

	cv::imshow("Task 6", img);
	cv::waitKey(0);

	return 0;
}