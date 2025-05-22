#include "Item.h"

Item::Item(const std::string& name, const std::string& description, bool isUsable)
    : name(name), description(description), isUsable(isUsable) {
}

std::string Item::use() {
    return isUsable ? "Using item: " + name : "Item is not usable";
}

std::string Item::getName() const {
    return this->name;
}