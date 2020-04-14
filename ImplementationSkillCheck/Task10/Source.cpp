#include <iostream>
#include <opencv2//opencv.hpp>


void binarize(int threshold, void* inputData)
{
	cv::Mat *img;
	img = (cv::Mat*)(inputData);
	cv::Mat output;
	cv::threshold(*img, output, threshold, 255, cv::THRESH_BINARY);
	cv::imshow("Binarize", output);
}

int main()
{
	// Task 10
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	cv::Mat imgGray;
	cv::cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);

	int threshold = 127;
	cv::namedWindow("Binarize", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("threshold", "Binarize", &threshold, 255, binarize, (void*)(&imgGray));
	
	binarize(threshold, (void*)(&imgGray));
	cv::waitKey(0);

	return 0;
}