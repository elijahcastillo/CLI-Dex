#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include <functional>
#include <sstream>
#include <unordered_map>

#include "Collection.h"
#include "Entry.h"

class TodoItem;

class FileHandler {
public:
    static void saveToFile(std::shared_ptr<Collection> root, const std::string& filename);
    static std::shared_ptr<Collection> readFromFile(const std::string& filename);
};
