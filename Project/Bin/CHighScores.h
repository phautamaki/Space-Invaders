#ifndef _CHIGHSCORES_H_
#define _CHIGHSCORES_H_

#include <vector>
#include <string>

//=============================================================================
class CHighScores {

public:
	CHighScores();

	bool OnInit();

	bool CheckPoints(int Points) const;
	void Add(std::string Name, int Points);

private:
	bool SavePoints();

//-----------------------------------------------------------------------------
private:
	// TODO: Use better container (or vector with struct)
	std::vector<int> ScoreList;
	std::vector<std::string> NameList;

	std::string FilePath;
	unsigned int MaxScores;
};

//=============================================================================

#endif