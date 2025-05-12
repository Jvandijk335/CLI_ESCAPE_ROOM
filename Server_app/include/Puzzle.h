#pragma once
#include <string>

class Puzzle {
public:
    Puzzle(const std::string& name, const std::string& description, const std::string& solution);
    bool attemptSolution(const std::string& input);
    std::string getClue();

private:
    std::string name;
    std::string description;
    std::string solution;
    bool isSolved;
};