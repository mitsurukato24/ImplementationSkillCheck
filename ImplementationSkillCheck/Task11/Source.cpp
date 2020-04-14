#include <iostream>
#include <opencv2//opencv.hpp>

void label(int threshold, void* inputData)
{
	cv::Mat img;
	cv::threshold(*(cv::Mat*)(inputData), img, threshold, 255, cv::THRESH_BINARY);

	// Labeling
	cv::Mat imgLabel(img.size(), CV_32S);
	int nLabels = cv::connectedComponents(img, imgLabel);

	// Decide colors
	std::vector<cv::Vec3b> colors(nLabels);
	colors[0] = cv::Vec3b(0, 0, 0);
	for (int i = 1; i < nLabels; i++)
	{
		colors[i] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
	}

	cv::Mat output(img.size(), CV_8UC3);
	for (int y = 0; y < output.rows; y++)
	{
		for (int x = 0; x < output.cols; x++)
		{
			int lb = imgLabel.at<int>(y, x);
			cv::Vec3b &pixel = output.at<cv::Vec3b>(y, x);
			pixel = colors[lb];
		}
	}

	cv::imshow("Label", output);
}

int main()
{
	// Task 11
	std::string filename = "../data/tsukuba.png";

	cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	if (img.empty()) return -1;

	int threshold = 127;
	cv::namedWindow("Label", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("threshold", "Label", &threshold, 255, label, (void*)(&img));

	label(threshold, (void*)(&img));
	cv::waitKey(0);

	return 0;
}