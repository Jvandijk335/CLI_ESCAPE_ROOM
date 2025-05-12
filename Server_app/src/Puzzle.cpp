#include "Puzzle.h"

Puzzle::Puzzle(const std::string& name, const std::string& description, const std::string& solution)
    : name(name), description(description), solution(solution), isSolved(false) {
}

bool Puzzle::attemptSolution(const std::string& input) {
    isSolved = (input == solution);
    return isSolved;
}

std::string Puzzle::getClue() {
    return "Clue: Try something with " + name;
}
