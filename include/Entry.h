#pragma once
#include "TodoItem.h"
#include <iostream>



class Entry : public TodoItem {

public:
    Entry(std::string data, int parentID, bool isCompleted = false) : TodoItem(data, ClassType::Entry, parentID, isCompleted) {};

};



