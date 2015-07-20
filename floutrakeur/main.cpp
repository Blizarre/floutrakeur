#include <opencv2/opencv.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "fileFinder.h"
#include "filtersMetrics.h"
#include <omp.h> 

using namespace std;
namespace fs = boost::filesystem;

void main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Usage:" << endl << argv[0] << " directoryName outputFileName" << endl;
		return;
	}

	string root = argv[1];
	string outputFileName = argv[2];

	if (! fs::exists(root) || ! fs::is_directory(root))
	{
		cout << "Invalid input directory: " << root;
		return;
	}

	FileFinder f(root);
	std::vector<string> imageList = f.findAll(
		[](string n) { 
			return boost::iends_with(n, L".jpg") || boost::iends_with(n, L".jpeg");
	});

	// old-style for loop to benefit from openmp on MSVC (even 2015 !!)
	#pragma omp parallel for
	for (int i = 0; i < imageList.size(); i++)
	{
		cv::Mat image = cv::imread(imageList[i], cv::IMREAD_GRAYSCALE);
		FiltersMetrics metrics(image);
		float dog = metrics.dog();
		float lap2Max = metrics.lap2Max();
		float lapVar = metrics.lapVar();
		float lapMax = metrics.lapMax();
		
		#pragma omp critical
		{
			cout << imageList[i] << " [" << omp_get_thread_num() << "] : " << endl;
			cout << "dog: " << dog << ", lapMax: " << lapMax;
			cout << ", lapVar: " << lapVar << ", lap2Max: " << lap2Max << endl;
		}
	}

	// break
	int test; std::cin >> test;
}