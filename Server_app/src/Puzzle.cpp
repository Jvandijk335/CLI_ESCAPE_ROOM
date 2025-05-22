#include "Puzzle.h"
#include <iostream>

Puzzle::Puzzle(const std::string& name, const std::string& description, const int& solution)
    : name(name), description(description), solution(solution), isSolved(false) {
}

bool Puzzle::attemptSolution(const std::string& input) {

    int numericSolution;
    try 
    {
        numericSolution = std::stoi(input);
    }
    catch (const std::invalid_argument& e){
        return false;
    }
    catch (const std::out_of_range& e){
        return false;
    }

    if (isSolved) return true;
    if (numericSolution == solution){
        isSolved = true;
        return true;
    }
    return false;
}

std::string Puzzle::getClue() {
    return "Clue: Try something with " + name;
}

std::string Puzzle::getName() const {
    return this->name;
}
