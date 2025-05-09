#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
    std::string use();

private:
    std::string name;
    std::string description;
    bool isUsable;
};

#endif