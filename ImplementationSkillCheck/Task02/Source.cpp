#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{
	// Task2
	std::string filename = "../data/blais.mp4";

	cv::VideoCapture video;
	if (!video.open(filename)) return -1;

	double fps = video.get(cv::CAP_PROP_FPS);
	double waiting_time = 1000.0 / fps;

	while (true)
	{
		cv::Mat img;
		video >> img;
		if (img.empty()) break;

		cv::imshow("Task 2", img);
		cv::waitKey(waiting_time);
	}
	video.release();
	return 0;
}