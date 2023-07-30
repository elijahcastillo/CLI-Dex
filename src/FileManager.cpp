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
            file << "Collection," << collection->getData() << "," << collection->getParentId() << "," << collection->getId() << "\n";

            for (const auto& subItem : collection->getItems()) {
                saveItem(subItem); // Recursively call saveItem for each sub-collection or entry
            }
        } else {
            file << "Entry," << item->getData() << "," << item->getParentId() << "," << "-1" << "\n";
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
        std::string type, dataStr, parentIdStr, itemIdStr;
        int parentId, itemId;

        // Read comma-separated values from the line
        std::getline(iss, type, ',');
        std::getline(iss, dataStr, ',');
        std::getline(iss, parentIdStr, ',');
        std::getline(iss, itemIdStr, ',');

        // Convert string values to integers
        parentId = std::stoi(parentIdStr);
        itemId = (type == "Collection") ? std::stoi(itemIdStr) : -1;

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
            auto entry = std::make_shared<Entry>(dataStr, parentId);
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









// std::shared_ptr<Collection> FileHandler::readFromFile(const std::string& filename) {
//     std::ifstream file(filename);

//     if (!file.is_open()) {
//         // Handle file opening error
//         return nullptr;
//     }

//     // Map to store Collection IDs and their corresponding shared pointers
//     std::unordered_map<int, std::shared_ptr<Collection>> collectionMap;

//     // Create the root Collection
//     collectionMap[-1] = std::make_shared<Collection>("Root", -1, 1971570665);

//     // Helper function to recursively read items
//     std::function<void(const std::string&, int, int)> readItem = [&](const std::string& data, int parentId, int itemId) {
//         std::cout << "Reading Item: " << data << ", parentId: " << parentId << ", itemId: " << itemId << std::endl;
//         if (itemId == -1) {
//             // Create an Entry
//             auto entry = std::make_shared<Entry>(data, parentId);
//             if (collectionMap.find(parentId) != collectionMap.end()) {
//                 auto& items = collectionMap[parentId]->getItems(); // Get a non-const reference to the items vector
//                 items.push_back(entry);
//             } else {
//                 // Handle error: Parent Collection not found
//                 // You can throw an exception or handle the error as per your application's requirements.
//             }
//         } else {
//             // Create a Collection
//             auto collection = std::make_shared<Collection>(data, parentId, itemId);
//             collectionMap[itemId] = collection;

//             if (parentId != -1) {
//                 if (collectionMap.find(parentId) != collectionMap.end()) {
//                     auto& items = collectionMap[parentId]->getItems(); // Get a non-const reference to the items vector
//                     items.push_back(collection);
//                 } else {
//                     // Handle error: Parent Collection not found
//                     // You can throw an exception or handle the error as per your application's requirements.
//                 }
//             }
//         }
//     };

//     std::string line;
//     while (std::getline(file, line)) {
//         std::istringstream iss(line);
//         std::string type, dataStr, parentIdStr, itemIdStr;
//         int parentId, itemId;

//         // Read comma-separated values from the line
//         std::getline(iss, type, ',');
//         std::getline(iss, dataStr, ',');
//         std::getline(iss, parentIdStr, ',');
//         std::getline(iss, itemIdStr, ',');

//         // Convert string values to integers
//         parentId = std::stoi(parentIdStr);
//         itemId = (type == "Collection") ? std::stoi(itemIdStr) : -1;

//         // Create TodoItem based on the type
//         readItem(dataStr, parentId, itemId);
//     }

//     // Close the file
//     file.close();

//     // Return the root Collection (parent ID should be -1 for the root)
//     return collectionMap[-1];
// }