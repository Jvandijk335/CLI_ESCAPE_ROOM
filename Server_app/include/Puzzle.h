#pragma once
#include <string>

class Puzzle {
public:
    Puzzle(const std::string& name, const std::string& description, const int& solution);
    bool attemptSolution(const std::string& input);
    std::string getClue();
    std::string getName() const;

private:
    std::string name;
    std::string description;
    int solution;
    bool isSolved;
};