#pragma once
#include <string>

class Item {
public:
    Item(const std::string& name, const std::string& description, bool isUsable);
    std::string use();

private:
    std::string name;
    std::string description;
    bool isUsable;
};