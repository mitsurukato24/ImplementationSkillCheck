#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task4
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	cv::Mat imgRGB;
	cv::cvtColor(img, imgRGB, cv::COLOR_BGR2RGB);

	cv::hconcat(std::vector<cv::Mat>{img, imgRGB}, img);
	cv::imshow("Task 4", img);
	cv::waitKey(0);

	return 0;
}