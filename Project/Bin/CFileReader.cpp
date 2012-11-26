#include <iostream>
#include <fstream>
#include <sstream>
#include "CFileReader.h"
#include "functions.h"
#include "Paths.h"

//=============================================================================
CFileReader CFileReader::FileReader;

//-----------------------------------------------------------------------------
CFileReader::CFileReader() {
}

//=============================================================================
bool CFileReader::CheckFileExistence(std::string& Filename)
{
    std::ifstream f(Filename.c_str());
    return f.is_open();
}

//-----------------------------------------------------------------------------
bool CFileReader::GetFile(std::string filename, std::ifstream& file)
{
	
	std::string path = PATH_MAPS+filename;

    const bool file_exists = CheckFileExistence(path);
    if (!file_exists) {
        error ("File "+ filename +" not found.");
		return false;
    }

	file.open(path.c_str());
	return true;
}

//-----------------------------------------------------------------------------
bool CFileReader::GetRows(std::string filename, std::vector<std::string>& container ) {
	std::vector<std::string> results;

	std::ifstream file;
	std::string line;

	// Try opening the file
	if(!CFileReader::FileReader.GetFile(filename, file)) {
		file.close();
		return false;
	}
	
	// Get all the rows
	while(!file.eof()) {
		std::getline(file, line);
		std::istringstream istream(line);
		// Ignore comments and empty lines
		if (istream.peek() != '#' && istream.peek() != 15 && line != "") {
			results.push_back(line);
		}
	}
	file.close();

	// Everything went well, give result
	container = results;
	return true;
}

//=============================================================================