#ifndef _CFILEREADER_H_
#define _CFILEREADER_H_

#include <vector>
#include <string>

//=============================================================================
class CFileReader {

public:

	static CFileReader FileReader;

	CFileReader();

	bool GetFile(std::string filename, std::ifstream& file);

	bool CheckFileExistence(std::string& Filename);

	bool GetRows(std::string Filename, std::vector<std::string>& Container );

private:

};

//=============================================================================

#endif