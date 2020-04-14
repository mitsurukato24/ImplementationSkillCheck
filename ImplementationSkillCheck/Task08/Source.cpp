#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task 8
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	// Gaussian noise
	cv::Mat imgGaussianNoise, imgSaltpepperNoise;
	imgGaussianNoise = img.clone();
	cv::randn(imgGaussianNoise, 0, 30);
	imgGaussianNoise = imgGaussianNoise + img;

	// Salt and pepper noise
	cv::Mat SaltpepperNoise = cv::Mat::zeros(img.rows, img.cols, CV_8U);
	cv::randu(SaltpepperNoise, 0, 255);
	cv::Mat salt, pepper;
	int range = 10;
	salt = SaltpepperNoise > 255 - range;
	pepper = SaltpepperNoise < range;
	imgSaltpepperNoise = img.clone();
	imgSaltpepperNoise.setTo(255, salt);
	imgSaltpepperNoise.setTo(0, pepper);

	cv::imshow("Task 8", img);
	cv::imshow("Gaussian Noise", imgGaussianNoise);
	cv::imshow("Salt-and-pepper Noise", imgSaltpepperNoise);
	cv::waitKey(0);

	return 0;
}