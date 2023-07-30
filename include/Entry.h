#pragma once
#include "TodoItem.h"
#include <iostream>



class Entry : public TodoItem {

public:
    Entry(std::string data, int parentID) : TodoItem(data, ClassType::Entry, parentID) {};

};



