#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task 9
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename);
	if (img.empty()) return -1;

	cv::resize(img, img, cv::Size(480, 320));

	// Gaussian noise
	cv::Mat imgGaussianNoise= img.clone();
	cv::randn(imgGaussianNoise, 0, 30);
	imgGaussianNoise = imgGaussianNoise + img;

	cv::Mat imgGaussianNoiseBlued,
		imgGaussianNoiseGaussianBlued,
		imgGaussianNoiseMedianBlued,
		imgGaussianNoiseBilateralFiltered;
	cv::blur(imgGaussianNoise, imgGaussianNoiseBlued, cv::Size(3, 3));
	cv::GaussianBlur(imgGaussianNoise, imgGaussianNoiseGaussianBlued, cv::Size(3, 3), 0);
	cv::medianBlur(imgGaussianNoise, imgGaussianNoiseMedianBlued, 3);
	int d = 15;
	int sigmaColor = 20;
	int sigmaSpace = 20;
	cv::bilateralFilter(imgGaussianNoise, imgGaussianNoiseBilateralFiltered, d, sigmaColor, sigmaSpace);

	cv::Mat imgGaussianNoiseBlured;
	std::vector<cv::Mat> imgs{
		imgGaussianNoise,
		imgGaussianNoiseBlued,
		imgGaussianNoiseGaussianBlued,
		imgGaussianNoiseMedianBlued,
		imgGaussianNoiseBilateralFiltered
	};
	cv::hconcat(imgs, imgGaussianNoiseBlured);

	// Salt and pepper noise
	cv::Mat saltpepperNoise = cv::Mat::zeros(img.rows, img.cols, CV_8U);
	cv::randu(saltpepperNoise, 0, 255);
	cv::Mat salt, pepper;
	int range = 10;
	salt = saltpepperNoise > 255 - range;
	pepper = saltpepperNoise < range;
	cv::Mat imgSaltpepperNoise = img.clone();
	imgSaltpepperNoise.setTo(255, salt);
	imgSaltpepperNoise.setTo(0, pepper);

	cv::Mat imgSaltpepperNoiseBlured,
		imgSaltpepperNoiseGaussianBlured,
		imgSaltpepperNoiseMedianBlured,
		imgSaltpepperNoiseBilateralFiltered;
	cv::blur(imgSaltpepperNoise, imgSaltpepperNoiseBlured, cv::Size(3, 3));
	cv::GaussianBlur(imgSaltpepperNoise, imgSaltpepperNoiseGaussianBlured, cv::Size(3, 3), 0);
	cv::medianBlur(imgSaltpepperNoise, imgSaltpepperNoiseMedianBlured, 3);
	cv::bilateralFilter(imgSaltpepperNoise, imgSaltpepperNoiseBilateralFiltered, d, sigmaColor, sigmaSpace);

	cv::Mat imgSaltpepperNoiseBlured;
	imgs = {
		imgSaltpepperNoise,
		imgSaltpepperNoiseBlured,
		imgSaltpepperNoiseGaussianBlured,
		imgSaltpepperNoiseMedianBlured,
		imgSaltpepperNoiseBilateralFiltered
	};
	cv::hconcat(imgs, imgSaltpepperNoiseBlured);

	cv::imshow("Task 9", img);
	cv::imshow("Gaussian Noise: added noise, blured, gaussian blured, median blured,  biratel fitered", imgGaussianNoiseBlured);
	cv::imshow("Salt-and-pepper Noise: added noise, blured, gaussian blured, median blured, biratel fitered", imgSaltpepperNoiseBlured);
	cv::waitKey(0);

	return 0;
}