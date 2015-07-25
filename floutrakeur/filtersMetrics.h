#pragma once

#include <opencv2/opencv.hpp>

class FiltersMetrics
{
public:
	FiltersMetrics(cv::Mat image);
	float dog();

	float lapMax();

	float lapVar();

	float lap2Max();

	float lapModMax();

    float waveletsMean(int NIter);

protected:
	cv::Mat m_smooth3; // image smoothed by a 3x3 gaussian kernel
	cv::Mat m_lapSmooth3; // laplacian of the 3x3 smoothed image
	cv::Mat m_image;
	int m_nbElts; // number of elements in the matrix
	const cv::Mat gauss3 = cv::getGaussianKernel(3, -1);
	const cv::Mat gauss5 = cv::getGaussianKernel(5, -1);
	const cv::Mat gauss9 = cv::getGaussianKernel(9, -1);
};

