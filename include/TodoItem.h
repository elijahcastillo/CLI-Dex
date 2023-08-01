#pragma once
#include <string>

class TodoItem
{

public:
    enum class ClassType { Collection, Entry };
     TodoItem(std::string data, ClassType type, int parentID = -1, bool isCompleted = false) : m_data(data), m_type(type), m_parentID(parentID), isCompleted(isCompleted) {}
    virtual ~TodoItem(){};

    ClassType getType() const { return m_type; }
    std::string getData() const { return m_data; }
    int getParentId() const { return m_parentID; }
    bool getIsCompleted() const { return isCompleted; }
    void updateIsCompleted(){ isCompleted = !isCompleted; }

private:
    std::string m_data;
    ClassType m_type;
    int m_parentID;
    bool isCompleted;

};


