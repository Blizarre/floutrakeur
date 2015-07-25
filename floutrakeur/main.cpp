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


int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cerr << "Usage:" << endl << argv[0] << " directoryName outputFileName class" << endl;
		return 0;
	}

	string root = argv[1];
	string outputFileName = argv[2];
	string dataClass = argv[3];

	if (! fs::exists(root) || ! fs::is_directory(root))
	{
		cerr << "Invalid input directory: " << root << endl;
		return 0;
	}

	auto output = shared_ptr<std::ostream>(new std::ofstream(outputFileName));
	CsvWriter csv(output, { "Class", "Wav mean", "DoG", "lap2Max", "lapVar", "lapMax", "fileName" });

	FileFinder f(root);

	std::vector<string> imageList = f.findAll(
		[](string n) {
			return boost::iends_with(n, L".jpg") || boost::iends_with(n, L".jpeg");
	});

	extractData(imageList, csv, dataClass);

    return 0;
}


void extractData(std::vector<std::string>& imageList, CsvWriter& csv, string dataClass)
{
	// old-style for loop to benefit from openmp on MSVC (even 2015 !!)
	#pragma omp parallel for
	for (int i = 0; i < imageList.size(); i++)
	{
		cv::Mat image = cv::imread(imageList[i], cv::IMREAD_GRAYSCALE);
        if(image.empty())
        {
            cerr << "Error opening image " << imageList[i] << endl;
        }
        else
        {
            FiltersMetrics metrics(image);
            float wavMean = metrics.waveletsMean(1);
            float dog = metrics.dog();
            float lap2Max = metrics.lap2Max();
            float lapVar = metrics.lapVar();
            float lapMax = metrics.lapMax();
            #pragma omp critical
            {
                csv.addData({ dataClass, std::to_string(wavMean), std::to_string(dog), std::to_string(lap2Max), std::to_string(lapVar), std::to_string(lapMax), imageList[i] });
                cout << imageList[i] << " [" << omp_get_thread_num() << "]" << endl;
            }
        }
	}
}
