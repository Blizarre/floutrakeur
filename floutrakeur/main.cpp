#include <opencv2/opencv.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "fileFinder.h"
#include "filtersMetrics.h"
#include <omp.h> 
#include <iostream>
#include <fstream>
#include "csvWriter.h"

using namespace std;
namespace fs = boost::filesystem;

// todo: create a class for this
void extractData(std::vector<std::string>& imageList, CsvWriter& csv, string dataClass);


void main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "Usage:" << endl << argv[0] << " directoryName outputFileName class" << endl;
		return;
	}

	string root = argv[1];
	string outputFileName = argv[2];
	string dataClass = argv[3];

	if (! fs::exists(root) || ! fs::is_directory(root))
	{
		cout << "Invalid input directory: " << root;
		return;
	}

	auto output = shared_ptr<std::ostream>(new std::ofstream(outputFileName));
	CsvWriter csv(output, { "Class", "DoG", "lap2Max", "lapVar", "lapMax" });

	FileFinder f(root);

	std::vector<string> imageList = f.findAll(
		[](string n) { 
			return boost::iends_with(n, L".jpg") || boost::iends_with(n, L".jpeg");
	});

	extractData(imageList, csv, dataClass);

	// break
	int test; std::cin >> test;
}


void extractData(std::vector<std::string>& imageList, CsvWriter& csv, string dataClass)
{
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
			csv.addData({ dataClass, std::to_string(dog), std::to_string(lap2Max), std::to_string(lapVar), std::to_string(lapMax) });
			cout << imageList[i] << " [" << omp_get_thread_num() << "]" << endl;
		}
	}
}