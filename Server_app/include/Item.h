#pragma once
#include <string>

class Item {
public:
    Item(const std::string& name, const std::string& description, bool isUsable);
    std::string use();
    std::string getName() const;

private:
    std::string name;
    std::string description;
    bool isUsable;
};