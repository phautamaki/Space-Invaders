#ifndef _CFILEREADER_H_
#define _CFILEREADER_H_

#include <vector>

class CFileReader {

public:

	static CFileReader FileReader;

	CFileReader();

	void GetFile(std::string filename, std::ifstream& file);

	bool CheckFileExistence(std::string& filename);

	std::string HV();

private:

};

#endif