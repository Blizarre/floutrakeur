#include "csvWriter.h"
#include <boost/algorithm/string.hpp>


CsvWriter::CsvWriter(std::shared_ptr<std::ostream> destination, std::vector<std::string> columnNames): m_destination(std::move(destination))
{
	m_columnLength = columnNames.size();
	(*m_destination) << boost::algorithm::join(columnNames, ",") << std::endl;
}

void CsvWriter::addData(std::vector<std::string> columnData)
{
	assert(m_columnLength == columnData.size());
	(*m_destination) << boost::algorithm::join(columnData, ",") << std::endl;
}
