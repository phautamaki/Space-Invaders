#include "CHighScores.h"
#include "CFileReader.h"
#include "CFont.h"
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

	NameList.clear();
	ScoreList.clear();

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
void CHighScores::OnRender(SDL_Surface* Surf_Display) const {
	if( ScoreList.size() == 0 ) return;

	int numX   = 100;
	int nameX  = 200;
	int pointX = 450;

	int startY = 50;

	int divider = 35;

	unsigned int maxNames = 10;

	for( unsigned int i = 0; i < ScoreList.size(); i++ ) {
		CFont::FontControl.Write(Surf_Display,IntToString(i+1).c_str(), numX, startY+(i*divider),1);
		CFont::FontControl.Write(Surf_Display,NameList.at(i).c_str(), nameX, startY+(i*divider),1);
		CFont::FontControl.Write(Surf_Display,IntToString(ScoreList.at(i)).c_str(), pointX, startY+(i*divider),1);

		if( i >= maxNames-1 ){
			break;
		}
	}
}

//=============================================================================
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
	std::string FullPath = "maps/" + FilePath;
	std::ofstream ScoreFile;
	ScoreFile.open(FullPath.c_str());

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