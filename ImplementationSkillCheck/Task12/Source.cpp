#include <iostream>
#include <opencv2//opencv.hpp>

#define ESC_KEY 27
#define SPACE_KEY 32

int main()
{
	// Task 12
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) return -1;
	// use Logicool C910
	cap.set(cv::CAP_PROP_AUTOFOCUS, 0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	int paramFocus = 15;
	cap.set(cv::CAP_PROP_FOCUS, paramFocus);  // 0~255
	
	enum Pattern {CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID};
	Pattern calibrationPattern = CHESSBOARD;

	cv::Mat frame;
	std::vector<std::vector<cv::Point2f> > cornerPts;  // corner points in image coordinates

	const cv::Size boardSize(10, 7);
	const float chessSize = 24.0;

	int minNumFrames = 10;
	while (true)
	{
		cap >> frame;

		bool found;
		std::vector<cv::Point2f> bufCornerPts;
		if (calibrationPattern == CHESSBOARD)
		{
			int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
			found = cv::findChessboardCorners(frame, boardSize, bufCornerPts, chessBoardFlags);
		}
		else if (calibrationPattern == CIRCLES_GRID)
		{
			found = cv::findCirclesGrid(frame, boardSize, bufCornerPts);
		}
		else if (calibrationPattern == ASYMMETRIC_CIRCLES_GRID)
		{
			found = cv::findCirclesGrid(frame, boardSize, bufCornerPts, cv::CALIB_CB_ASYMMETRIC_GRID);
		}

		if (found)
		{
			if (calibrationPattern == CHESSBOARD)
			{
				// the detected position is not accurate so use CornerSubPix to improve accuracy
				cv::Mat frameGray;
				cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
				int winSize = 11;  // half of search window
				cv::TermCriteria termCriteia(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 50, 0.0001);
				cv::cornerSubPix(frameGray, bufCornerPts, cv::Size(winSize, winSize), cv::Size(-1, -1), termCriteia);
			}

			cv::drawChessboardCorners(frame, boardSize, cv::Mat(bufCornerPts), found);
			cv::imshow("calibration", frame);
			int key = cv::waitKey(1);
			if (key == ESC_KEY) break;
			else if (key == SPACE_KEY)
			{
				cv::imshow("show picked image", frame);
				int k = cv::waitKey(0);
				if (k == SPACE_KEY) cornerPts.push_back(bufCornerPts);
			}
		}
		else
		{
			cv::imshow("calibration", frame);
			int key = cv::waitKey(1);
			if (key == ESC_KEY) break;
		}
	}

	// --- calculate intrinsic and extrinsic parameters
	if ((int)cornerPts.size() >= minNumFrames)
	{
		std::vector<cv::Point3f> newObjPts;
		std::vector<std::vector<cv::Point3f>> objPts(1);  // true 3d coordinates of corners
		for (int h = 0; h < boardSize.height; h++)
		{
			for (int w = 0; w < boardSize.width; w++)
			{
				objPts[0].push_back(cv::Point3f(chessSize*w, chessSize*h, 0));
			}
		}
		newObjPts = objPts[0];
		objPts.resize(cornerPts.size(), objPts[0]); // copy
		std::vector<cv::Mat> rvecs, tvecs;
		cv::Mat cameraMat = cv::Mat::eye(3, 3, CV_64F);
		cv::Mat distCoeff = cv::Mat::zeros(8, 1, CV_64F);
		cv::Size imgSize = frame.size();
		bool useCalibrateCameraRO = true;
		double rms;
		if (useCalibrateCameraRO)
		{
			// more accurate
			int iFixedPt = boardSize.width - 1;
			rms = cv::calibrateCameraRO(
				objPts, cornerPts, imgSize, iFixedPt, cameraMat, distCoeff,
				rvecs, tvecs, newObjPts
			);
		}
		else
		{
			rms = cv::calibrateCamera(
				objPts, cornerPts, imgSize, cameraMat, distCoeff, rvecs, tvecs
			);
		}
		std::cout << "RMS error reported by calibrateCamera: " << rms << std::endl;

		if (!(cv::checkRange(cameraMat) && cv::checkRange(distCoeff)))
		{
			std::cout << "Calibration failed" << std::endl;
			return -1;
		}

		// --- caliculate avg reprojection error
		objPts.clear();
		objPts.resize(cornerPts.size(), newObjPts);
		double totalErr = 0, err;
		int totalPts = 0;
		std::vector<float> reprojErrs;
		std::vector<cv::Point2f> tmpImgPts;
		reprojErrs.resize(objPts.size());
		for (int i = 0; i < (int)objPts.size(); i++)
		{
			cv::projectPoints(cv::Mat(objPts[i]), rvecs[i], tvecs[i], cameraMat, distCoeff, tmpImgPts);
			err = cv::norm(cv::Mat(cornerPts[i]), cv::Mat(tmpImgPts), cv::NORM_L2);
			int n = (int)objPts[i].size();
			reprojErrs[i] = (float)std::sqrt(err*err / n);
			totalErr += err*err;
			totalPts += n;
		}
		double totalAvgErr = std::sqrt(totalErr / totalPts);
		printf("Calibration succeeded, avg reprojection error = %.7f\n", totalAvgErr);

		// --- save as yml file
		time_t now = time(nullptr);
		struct tm pnow;
		localtime_s(&pnow, &now);
		char date[50];
		sprintf_s(date, "%02d%02d%02d%02d%02d", pnow.tm_mon + 1,
			pnow.tm_mday, pnow.tm_hour, pnow.tm_min, pnow.tm_sec);

		std::string filename = "output_camera_data_" + std::string(date) + ".yml";
		cv::FileStorage fs(filename, cv::FileStorage::WRITE);
		if (!fs.isOpened())
		{
			std::cout << "File can not be opened. " << std::endl;
			return -1;
		}
		fs << "calibration_time" << date;
		fs << "camera_focus_parameter" << paramFocus;
		fs << "image_width" << imgSize.width;
		fs << "image_height" << imgSize.height;
		fs << "board_width" << boardSize.width;
		fs << "board_height" << boardSize.height;
		fs << "cell_size" << chessSize;
		fs << "camera_matrix" << cameraMat;
		fs << "tvecs" << tvecs;
		fs << "rvecs" << rvecs;
		fs << "distortion_error" << distCoeff;
		fs << "avg_reprojection_error" << totalAvgErr;
		fs << "per_view_reprojection_errors" << cv::Mat(reprojErrs);

		fs.release();

		// --- show undistorted images
		cv::Mat map1, map2;
		cv::initUndistortRectifyMap(cameraMat, distCoeff, cv::Mat(),
			cv::getOptimalNewCameraMatrix(cameraMat, distCoeff, imgSize, 1, imgSize, 0),
			imgSize, CV_16SC2, map1, map2);

		cv::Mat frameUndistorted;
		while (true)
		{
			cap >> frame;
			cv::remap(frame, frameUndistorted, map1, map2, cv::INTER_LINEAR);
			cv::resize(frame, frame, frameUndistorted.size());
			cv::hconcat(std::vector<cv::Mat>{ frame, frameUndistorted }, frame);
			cv::imshow("undistortion", frame);
			int kk = cv::waitKey(1);
			if (kk == ESC_KEY) break;
		}
	}
	else
	{
		std::cout << "number of images is not enough" << std::endl;
	}
	return 0;
}