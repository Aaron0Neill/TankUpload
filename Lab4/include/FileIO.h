#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "yaml-cpp\yaml.h"


class FileIO
{
public:
	struct Stats
	{
		Stats(int s, std::string p, int a) : 
			m_score(s), 
			m_playerName(p),
			m_accuracy(a) 
		{}
		int m_score;
		std::string m_playerName;
		int m_accuracy;
	};

	static std::vector<Stats> getHighScore();

	FileIO(Stats& playerStats);
private:
	void readHighScore();
	void writeHighScore();
	
	int const c_TABLE_SIZE{ 10 };

	Stats m_playerStats;
	std::vector<Stats> m_tableStats;
	YAML::Node m_outputNode;
};

