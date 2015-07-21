#pragma once

#include <vector>
#include <string>
#include <memory>

class CsvWriter
{
public:
	CsvWriter(std::shared_ptr<std::ostream> destination, std::vector<std::string> columnNames);
	void addData(std::vector<std::string> columnData);

protected:
	std::shared_ptr<std::ostream> m_destination;
	std::size_t m_columnLength;
};

