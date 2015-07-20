#include "filtersMetrics.h"
#include <cmath>


FiltersMetrics::FiltersMetrics(cv::Mat image): m_image(image)
{
	assert(m_image.dims == 2);
	m_nbElts = image.size[0] * image.size[1];

	cv::sepFilter2D(image, m_smooth3, CV_32F, gauss3, gauss3);
	cv::Laplacian(m_smooth3, m_lapSmooth3, CV_32F, 1);
}

/**
 * return the means of Difference of gaussian between a 3x3 and a 9x9 gaussian blur
 */
float FiltersMetrics::dog()
{
	cv::Mat smooth9;
	cv::sepFilter2D(m_image, smooth9, CV_32F, gauss9, gauss9);
	cv::Mat diff = smooth9 - m_smooth3;
	cv::multiply(diff, diff, diff); // diff = (smooth9 - m_smooth3) .* (smooth9 - m_smooth3)
	return static_cast<float>(cv::sum(diff)[0]);
}

/**
 * Helper function used to get the 99% maximum value of an array
 */
float get99perc(cv::Mat image)
{
	assert(image.dims == 2);
	cv::Mat line, sortedLine;
	int nbElts = image.size[0] * image.size[1];
	int perc99Element = static_cast<int>(std::floor(nbElts * 0.99));

	line = image.reshape(1, nbElts);
	assert(line.size[1] == 1);

	cv::sort(line, sortedLine, CV_SORT_EVERY_COLUMN);

	// check that sorting is working (different elements, and in the right order)
	assert(sortedLine.at<float>(perc99Element) < sortedLine.at<float>(nbElts - 1));
	assert(sortedLine.at<float>(0) < sortedLine.at<float>(perc99Element));

	return sortedLine.at<float>(perc99Element);
}

/**
* return the 99% max of the laplacian
*/
float FiltersMetrics::lapMax()
{
	return get99perc(m_lapSmooth3);
}

/**
* return the variance of the laplacian
*/
float FiltersMetrics::lapVar()
{
	cv::Scalar mean, stdDev;
	cv::meanStdDev(m_lapSmooth3, mean, stdDev);
	return static_cast<float>(stdDev[0]);
}

/**
 * Modified laplacian maximum (99%)
 * OpenCV port of 'LAPM' algorithm (Nayar89)
 * http://stackoverflow.com/questions/7765810/is-there-a-way-to-detect-if-an-image-is-blurry
 */
float FiltersMetrics::lap2Max()
{
	cv::Mat M = (cv::Mat_<double>(3, 1) << -1, 2, -1);
	cv::Mat G = cv::getGaussianKernel(3, -1, CV_32F);

	cv::Mat Lx;
	cv::sepFilter2D(m_image, Lx, CV_32F, M, G);

	cv::Mat Ly;
	cv::sepFilter2D(m_image, Ly, CV_32F, G, M);

	cv::Mat FM = cv::abs(Lx) + cv::abs(Ly);

	return get99perc(FM);
}


FiltersMetrics::~FiltersMetrics()
{
}
