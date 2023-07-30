#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <random>


#include "Entry.h"

class TodoItem;



class Collection : public TodoItem {
public:
    Collection(std::string data, int parentId, int id) : TodoItem(data, ClassType::Collection, parentId), id(id) {}
    Collection(std::string data, int parentId) : TodoItem(data, ClassType::Collection, parentId), id(generateRandomInt()) {}

    //Maybe add another constructor that automatically creates the id with a UUID this would be used when creating a new collection
    //But the other Collection above would be used when loading in the collection from the file since we would already know the id

    //API
    void createEntry(std::string data);
    void createSubCollection(std::string data);

    std::shared_ptr<Collection> getSubCollection(int index);

    //Utils
    int generateRandomInt();

    //Getters
    int getId() const { return id; }
    std::vector<std::shared_ptr<TodoItem>>& getItems() { return items; }

private:
    int id; // Unique ID for the Collection
    std::vector<std::shared_ptr<TodoItem>> items;
};





