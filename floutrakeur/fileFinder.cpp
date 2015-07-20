#include "fileFinder.h"

using namespace boost::filesystem;
using namespace std;

void findRec(std::function<bool(string)> filter, vector<string>& matchingFiles, path& currentPath);


FileFinder::FileFinder(string rootPath): m_rootPath(rootPath) { }

std::vector<string> FileFinder::findAll(std::function<bool(string)> filter)
{
	std::vector<string> matchingFiles;
	findRec(filter, matchingFiles, m_rootPath);
	return matchingFiles;
}

void findRec(std::function<bool(string)> filter, vector<string>& matchingFiles, path& currentPath)
{
	for (path p : directory_iterator(currentPath))
	{
		if (is_directory(p))
		{
			findRec(filter, matchingFiles, p);
		}
		else
			if (filter(p.filename().generic_string()))
			{
				matchingFiles.push_back( p.string() );
			}
	}
}
