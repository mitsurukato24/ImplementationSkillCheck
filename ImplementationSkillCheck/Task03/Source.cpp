#include <iostream>
#include <direct.h>
#include <opencv2//opencv.hpp>

int main()
{
	// Task3
	std::string filename = "../data/blais.mp4";

	cv::VideoCapture video;
	if (!video.open(filename)) return -1;
	
	const char *outputDir = "./output";
	struct stat statBuf;
	if (stat(outputDir, &statBuf) != 0)
	{
		_mkdir(outputDir);
	}
	
	int width = video.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = video.get(cv::CAP_PROP_FRAME_HEIGHT);
	cv::VideoWriter writer("./output/task3_%04d.jpg", 0, 0.0, cv::Size(width, height));
	if (!writer.isOpened()) return -1;

	int counter = 0;
	while (true)
	{
		cv::Mat img;
		video >> img;
		if (img.empty()) break;
		writer << img;
		std::cout << "image : " << counter << std::endl;
		counter++;
	}
	video.release();
	writer.release();
	return 0;
}