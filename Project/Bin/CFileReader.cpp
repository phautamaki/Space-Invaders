#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CFileReader.h"
#include "functions.h"
#include "Paths.h"

CFileReader CFileReader::FileReader;

CFileReader::CFileReader() {
}

bool CFileReader::CheckFileExistence(std::string& filename)
{
    std::ifstream f(filename.c_str());
    return f.is_open();
}

void CFileReader::GetFile(std::string filename, std::ifstream& file)
{
	
	std::string path = PATH_MAPS+filename;

    const bool file_exists = CheckFileExistence(path);
    if (!file_exists) {
        debug ("File "+ filename +" not found.");
    }

	file.open(path.c_str());
}
