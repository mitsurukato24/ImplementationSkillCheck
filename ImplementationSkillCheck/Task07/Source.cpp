#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task 7
	std::string filename = "../data/tsukuba.png";

	cv::Mat img, imgGray;
	img = cv::imread(filename);
	if (img.empty()) return -1;

	// Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
	// cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

	cv::cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);

	cv::Mat imgSobelx, imgSobely, imgSobel, imgLaplacian, imgCanny;
	cv::Sobel(imgGray, imgSobelx, CV_16S, 1, 0);
	cv::Sobel(imgGray, imgSobely, CV_16S, 0, 1);
	cv::convertScaleAbs(imgSobelx, imgSobelx);
	cv::convertScaleAbs(imgSobely, imgSobely);
	cv::addWeighted(imgSobelx, 0.5, imgSobely, 0.5, 0, imgSobel);

	int kernelSize = 3;
	cv::Laplacian(imgGray, imgLaplacian, CV_16S, kernelSize);
	// convert back to CV_8U
	cv::convertScaleAbs(imgLaplacian, imgLaplacian);

	cv::Canny(imgGray, imgCanny, 50, 150);
	imgCanny.convertTo(imgCanny, CV_8U);

	cv::imshow("Task 7", img);
	cv::imshow("Sobel", imgSobel);
	cv::imshow("Laplacian", imgLaplacian);
	cv::imshow("Canny", imgCanny);
	cv::waitKey(0);

	return 0;
}