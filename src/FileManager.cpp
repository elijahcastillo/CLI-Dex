#include "FileManager.h"

class Collection;
class Entry;

void FileHandler::saveToFile(std::shared_ptr<Collection> root, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        // Handle file opening error
        return;
    }

    // Helper function to recursively save items
    std::function<void(const std::shared_ptr<TodoItem>&)> saveItem = [&](const std::shared_ptr<TodoItem>& item) {
        if (item->getType() == TodoItem::ClassType::Collection) {
            auto collection = std::static_pointer_cast<Collection>(item);
            file << "Collection," << collection->getData() << "," << collection->getParentId() << "," << collection->getId() << "," << item->getIsCompleted() << "\n";

            for (const auto& subItem : collection->getItems()) {
                saveItem(subItem); // Recursively call saveItem for each sub-collection or entry
            }
        } else {
            file << "Entry," << item->getData() << "," << item->getParentId() << "," << "-1"  << "," << item->getIsCompleted() << "\n";
        }
    };

    saveItem(root); // Start the recursive saving process from the root collection

    file.close();
}



std::shared_ptr<Collection> FileHandler::readFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        // Handle file opening error
        return nullptr;
    }

    // Map to store Collection IDs and their corresponding shared pointers
    std::unordered_map<int, std::shared_ptr<Collection>> collectionMap;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type, dataStr, parentIdStr, itemIdStr, isCompletedStr;
        int parentId, itemId, isCompleted;

        // Read comma-separated values from the line
        std::getline(iss, type, ',');
        std::getline(iss, dataStr, ',');
        std::getline(iss, parentIdStr, ',');
        std::getline(iss, itemIdStr, ',');
        std::getline(iss, isCompletedStr, ',');



        // Convert string values to integers
        try {
            // This code may throw an exception
            parentId = std::stoi(parentIdStr);
            itemId = (type == "Collection") ? std::stoi(itemIdStr) : -1;
            isCompleted = (isCompletedStr == "1") ? true : false; //When bool is converted to string it is "1" or "0"

        } catch (const std::exception& e) {
        // This code will be executed if an exception is thrown
        std::cerr << "An exception was thrown: " << e.what() << std::endl;
        } 


        // Create TodoItem based on the type
        if (collectionMap.find(parentId) == collectionMap.end()) {
            // If the parent collection does not exist, create it
            if (parentId == -1) {
                collectionMap[parentId] = std::make_shared<Collection>("Root", parentId, itemId);
            } else {
                collectionMap[parentId] = std::make_shared<Collection>("TempParent", parentId, itemId);
            }
        }

        if (itemId == -1) {
            // Create an Entry
            auto entry = std::make_shared<Entry>(dataStr, parentId, isCompleted);
            collectionMap[parentId]->getItems().push_back(entry);
        } else {
            // Create a Collection
            auto collection = std::make_shared<Collection>(dataStr, parentId, itemId);
            collectionMap[itemId] = collection;
            collectionMap[parentId]->getItems().push_back(collection);
        }
    }

    // Close the file
    file.close();

    // Return the root Collection (parent ID should be -1 for the root)
    return collectionMap[-1];
}


