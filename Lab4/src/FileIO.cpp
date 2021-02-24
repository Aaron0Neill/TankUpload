#include "FileIO.h"

std::vector<FileIO::Stats> FileIO::getHighScore()
{
	try
	{
		YAML::Node highScores = YAML::LoadFile((".//resources//HighScores//HighScores.yaml"));
		std::vector<FileIO::Stats> scoreTable;
		if (highScores.IsNull())
		{ // if the yaml file is open
			throw std::exception("Error loading HighScores");
		}
		if (highScores["table"].IsDefined())
		{
			const YAML::Node& seq = highScores["table"].as<YAML::Node>();
			for (auto& s : seq)
			{ // loop through all the sequences in the table and store them in the vector
				Stats ts(s["highScore"]["score"].as<int>(), s["highScore"]["name"].as<std::string>(), s["highScore"]["accuracy"].as<int>());
				scoreTable.push_back(ts);
			}
			return scoreTable;
		}
	}
	catch (YAML::ParserException & e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception & e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}
}

FileIO::FileIO(Stats& playerStats) :
	m_playerStats(playerStats),
	m_outputNode(YAML::NodeType::Sequence)
{
	readHighScore();
	writeHighScore();
}

//################################################################################################################b

void FileIO::readHighScore()
{
	try
	{
		// open the yaml file
		m_outputNode = YAML::LoadFile(".//resources//HighScores//HighScores.yaml");
		if (m_outputNode.IsNull())
		{ // if the yaml file is open
			throw std::exception("Error loading HighScores");
		}
		if (m_outputNode["table"].IsDefined())
		{
			const YAML::Node& seq = m_outputNode["table"].as<YAML::Node>();
			for (auto& s : seq)
			{ // loop through all the sequences in the table and store them in the vector
				Stats ts(s["highScore"]["score"].as<int>(), s["highScore"]["name"].as<std::string>(), s["highScore"]["accuracy"].as<int>());
				m_tableStats.push_back(ts);
			}
		}
	}
	catch (YAML::ParserException & e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception & e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}
}

//################################################################################################################b

void FileIO::writeHighScore()
{
	YAML::Emitter newFile;
	YAML::Node scoreNode(YAML::NodeType::Sequence);
	bool scoreAdded{ false }; // controls if the score is added
	for (int i = 0; i < m_tableStats.size(); i++)
	{
		if (!scoreAdded)
		{ // make sure the score hasnt been added already
			if (m_playerStats.m_score > m_tableStats[i].m_score)
			{ // the player needs to be added here
				scoreAdded = true;
				YAML::Node score(YAML::NodeType::Map);
				score["highScore"]["name"] = m_playerStats.m_playerName;
				score["highScore"]["score"] = m_playerStats.m_score;
				score["highScore"]["accuracy"] = m_playerStats.m_accuracy;
				scoreNode["table"].push_back(score);
				i--;
			}
			else
			{ // add the same line to the new file
				YAML::Node score(YAML::NodeType::Map);
				score["highScore"]["name"] = m_tableStats[i].m_playerName;
				score["highScore"]["score"] = m_tableStats[i].m_score;
				score["highScore"]["accuracy"] = m_tableStats[i].m_accuracy;
				scoreNode["table"].push_back(score);
			}
		}
		else
		{
			YAML::Node score(YAML::NodeType::Map);
			score["highScore"]["name"] = m_tableStats[i].m_playerName;
			score["highScore"]["score"] = m_tableStats[i].m_score;
			score["highScore"]["accuracy"] = m_tableStats[i].m_accuracy;
			scoreNode["table"].push_back(score);
		}
	}
	if (!scoreAdded)
	{ // if the players score wasnt added at all
		if (m_tableStats.size() < c_TABLE_SIZE)
		{// if the table is less than the full size of the table add the score
			YAML::Node score(YAML::NodeType::Map);
			score["highScore"]["name"] = m_playerStats.m_playerName;
			score["highScore"]["score"] = m_playerStats.m_score;
			score["highScore"]["accuracy"] = m_playerStats.m_accuracy;
			scoreNode["table"].push_back(score);
		}
	}
	newFile << scoreNode;
	std::ofstream output(".//resources//HighScores//HighScores.yaml");
	if (output.is_open())
		output << newFile.c_str();
	output.close();
}