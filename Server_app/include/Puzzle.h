#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>

class Puzzle {
public:
    bool attemptSolution(const std::string& input);
    std::string getClue();

private:
    std::string name;
    std::string description;
    std::string solution;
    bool isSolved;
};

#endif