#pragma once
#include <string>
#include <boost/filesystem.hpp>

class FileFinder
{
public:
	FileFinder(std::string rootPath);
	std::vector<std::string> findAll(std::function<bool(std::string)> filter);

protected:
	boost::filesystem::path m_rootPath;
};

