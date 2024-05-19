#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

class CQotd
{
public:
    // Constructor that takes the path to the quotes file
    CQotd(const std::string& filePath)
    {
        // Seed the random number generator
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Load quotes from the specified file
        LoadQuotes(filePath);
    }

    // Method to get a random quote
    std::string GetRandomQuote()
    {
        if (m_quotes.empty())
        {
            return "No quotes available.";
        }

        int randomIndex = std::rand() % m_quotes.size();
        return m_quotes[randomIndex];
    }

private:
    // Method to load quotes from the file
    void LoadQuotes(const std::string& filePath)
    {
        std::ifstream file(filePath);
        std::string line;

        if (file.is_open())
        {
            while (std::getline(file, line))
            {
                if (!line.empty())
                {
                    m_quotes.push_back(line);
                }
            }
            file.close();
        }
    }

    // Vector to store the quotes
    std::vector<std::string> m_quotes;
};
