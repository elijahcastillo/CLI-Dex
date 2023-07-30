#include "Collection.h"





int Collection::generateRandomInt() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
    return dis(gen);
}

void Collection::createEntry(std::string data){
    items.push_back(std::make_shared<Entry>(data, getId()));
}

void Collection::createSubCollection(std::string data){
    items.push_back(std::make_shared<Collection>(data, getId()));

};

std::shared_ptr<Collection> Collection::getSubCollection(int index){

    if(items[index]->getType() == TodoItem::ClassType::Entry){
        std::cout << "Cannot access Entry as Collection" << std::endl;

    }

    auto collection = std::static_pointer_cast<Collection>(items[index]);
    return collection;
};



