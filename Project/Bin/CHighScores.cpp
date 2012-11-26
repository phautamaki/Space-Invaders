#include "CHighScores.h"
#include "CFileReader.h"
#include "functions.h"
#include <fstream>
#include <cstdlib>

//=============================================================================
CHighScores::CHighScores() {
	FilePath = "HighScores.txt";
	MaxScores = 15;
}

//=============================================================================
bool CHighScores::OnInit() {
	std::vector<std::string> Rows;
	if(!CFileReader::FileReader.GetRows(FilePath,Rows)){
		return false;
	}

	// Get scores
	std::vector<std::string> RowValues;
	for( unsigned int i = 0; i < Rows.size(); i++ ) {
		RowValues = Explode(Rows.at(i),';');
		if( RowValues.size() == 2 ) {
			NameList.push_back(RowValues.at(0));
			ScoreList.push_back(StringToInt(RowValues.at(1)));
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
bool CHighScores::CheckPoints(int Points) const {
	if( ScoreList.at(ScoreList.size()-1) >= Points ) {
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
void CHighScores::Add(std::string Name, int Points) {
	unsigned int WhereTo = 0;
	for( unsigned int i = 0; i < ScoreList.size(); i++ ) {
		if( ScoreList.at(i) < Points ){
			WhereTo = i;
			break;
		}
	}

	NameList.insert(NameList.begin() + WhereTo, Name);
	ScoreList.insert(ScoreList.begin() + WhereTo, Points);

	if( SavePoints() ){
		debug("High score saved.");
	}
	else {
		error("Couldn't save high scores.");
	}
}
//=============================================================================
bool CHighScores::SavePoints() {
	std::ofstream ScoreFile;
	ScoreFile.open(FilePath.c_str());

	if(!ScoreFile) {
		error("Couldn't open high score list.");
		return false;
	}

	for( unsigned int i = 0; i < ScoreList.size(); i++ ) {
		if( i == MaxScores ) break;

		ScoreFile << NameList.at(i) << ';' << ScoreList.at(i) << std::endl;
	}


	ScoreFile.close();

	return true;
}

//=============================================================================