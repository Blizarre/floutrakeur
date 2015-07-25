#include "fileFinder.h"

using namespace boost::filesystem;
using namespace std;


FileFinder::FileFinder(string rootPath): m_rootPath(rootPath) { }

std::vector<string> FileFinder::findAll(std::function<bool(string)> filter)
{
	std::vector<string> matchingFiles;
	for (path p : recursive_directory_iterator(m_rootPath))
	{
        if (is_regular_file(p) && filter(p.filename().generic_string()))
        {
            matchingFiles.push_back( p.string() );
        }
	}
	return matchingFiles;
}
