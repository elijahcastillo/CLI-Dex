#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include <deque>
#include "Collection.h"
#include "FileManager.h"



// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"



class InteractiveConsole {
public:
    InteractiveConsole(std::shared_ptr<Collection> root_collection) : current_collection(root_collection), root_collection(root_collection), running(true) {
        console = GetStdHandle(STD_OUTPUT_HANDLE);
    }



    void displayMenu() {

        // Clear the screen before displaying the menu
        // system("cls");

        int start_y = 7;
        gotoXY(12, start_y - 4);
        std::cout << "===================================";
        gotoXY(12, start_y - 3);
        std::cout << "   ";
        for (auto it = collection_path.rbegin(); it != collection_path.rend(); ++it) {
            std::cout <<  (*it)->getData() << " / ";
        }
        std::cout << current_collection->getData();
        gotoXY(12, start_y - 2);
        std::cout << "===================================";

        
        for (size_t i = 0; i < current_collection->getItems().size(); ++i) {
            gotoXY(12, start_y);
            if(current_collection->getItems()[i]->getType() == TodoItem::ClassType::Collection){
                //Draw Collection
                std::cout << " ~  " << current_collection->getItems()[i]->getData();
            } else {
                //Draw Entry
                bool isCompleted = current_collection->getItems()[i]->getIsCompleted();
                if(isCompleted){
                    std::cout << "[" << GREEN << (isCompleted ? "x" : " ") << RESET << "] " << current_collection->getItems()[i]->getData();

                } else {
                    std::cout << "[" << (isCompleted ? "x" : " ") << "] " << current_collection->getItems()[i]->getData();

                }

            }

            ++start_y;
        }

        if(current_collection->getItems().size() == 0){
            gotoXY(12, start_y);
            std::cout << "Collection is Empty, nothing to see here";
            start_y++;
        }


        // Display Options at the end of menu
        start_y++;
        gotoXY(12, start_y);
        std::cout << "(N) Create New Entry";

        start_y++;
        gotoXY(12, start_y);
        std::cout << "(C) Create New Collection";

        start_y++;
        gotoXY(12, start_y);
        std::cout << "(D) Delete Selected";

        start_y++;
        gotoXY(12, start_y);
        std::cout << "(B) Go Back";

        start_y++;
        gotoXY(12, start_y);
        std::cout << "(Q) Quit & Save";



        // Redraw the arrow at the correct position
        gotoXY(9, 7 + x);
        std::cout << "->";
    }

    

    void run() {
        system("cls");
        while (running) {
            displayMenu();
            handleInput();
        }
    }

private:
    HANDLE console;
    COORD CursorPosition;
    int x = 0;
    int menu_item = 0;
    bool running;
    std::shared_ptr<Collection> root_collection;
    std::shared_ptr<Collection> current_collection;
    std::deque<std::shared_ptr<Collection>> collection_path;





    void goToSubCollection(std::shared_ptr<Collection> sub_collection){
        //Save parent collection
        collection_path.push_front(current_collection);
        

        //Change current collection
        current_collection = sub_collection;


        //Reset Arrow Position
        menu_item = 0;
        x = 0;

        // Clear the console and redraw the menu after adding the new entry
        system("cls");
        displayMenu();
    }

    void gotoXY(int x, int y) {
        CursorPosition.X = x;
        CursorPosition.Y = y;
        SetConsoleCursorPosition(console, CursorPosition);
    }

    void handleInput() {
        system("pause>nul");



        if (GetKeyState(VK_DOWN) & 0x8000) {
            gotoXY(9, 7 + x);
            std::cout << "  ";
            x = (x + 1) % current_collection->getItems().size();
            gotoXY(9, 7 + x);
            std::cout << "->";
            menu_item = x;
            return;
        } 


        if (GetKeyState(VK_UP) & 0x8000) {
            // std::cout << "&T!!!!!";
            gotoXY(9, 7 + x);
            std::cout << "  ";
            x = (x + (current_collection->getItems().size()) - 1) % current_collection->getItems().size();

            gotoXY(9, 7 + x);
            std::cout << "->";
            menu_item = x;

            return;
        }


        if (GetKeyState(VK_RETURN) & 0x8000) {

            if(current_collection->getItems()[menu_item]->getType() == TodoItem::ClassType::Collection){
                auto collection = std::static_pointer_cast<Collection>(current_collection->getItems()[menu_item]);
                goToSubCollection(collection);

            } else {
                //Change state of Entry
                current_collection->getItems()[menu_item]->updateIsCompleted();
            }

            return;

        }







        // Check for 'N' key (78 in ASCII)
        if (GetKeyState(78) & 0x8000) {

            gotoXY(50, 10);
            std::cout << "You chose to Create New Entry...     ";

            // Prompt the user to enter the data for the new entry
            gotoXY(50, 12);
            std::cout << "Enter the data for the new entry: ";
            std::string newEntryData;
            std::getline(std::cin, newEntryData);

            // Clear the lines used for the prompt
            gotoXY(50, 10);
            std::cout << std::setfill(' ') << std::setw(32) << ' '; //Remove text ("You chose to Create New Entry...")
            gotoXY(50, 12);
            std::cout << std::setfill(' ') << std::setw(34 + newEntryData.length()) << ' '; //Remove text ("Enter the data for the new entry: ")

            // Create a new entry with the entered data and add it to the current collection
            current_collection->createEntry(newEntryData);

            // Clear the console and redraw the menu after adding the new entry
            system("cls");
            displayMenu();

            return;

        }


        // Check for 'C' key (67 in ASCII)
        if (GetKeyState(67) & 0x8000) {

            gotoXY(50, 10);
            std::cout << "You chose to Create New Collection...";

            // Prompt the user to enter the data for the new entry
            gotoXY(50, 12);
            std::cout << "Enter the data for the new Collection: ";
            std::string newEntryData;
            std::getline(std::cin, newEntryData);

            // Clear the lines used for the prompt
            gotoXY(50, 10);
            std::cout << std::setfill(' ') << std::setw(37) << ' '; //Remove text ("You chose to Create New Entry...")
            gotoXY(50, 12);
            std::cout << std::setfill(' ') << std::setw(39 + newEntryData.length()) << ' '; //Remove text ("Enter the data for the new entry: ")

            // Create a new entry with the entered data and add it to the current collection
            current_collection->createSubCollection(newEntryData);

            // Clear the console and redraw the menu after adding the new entry
            system("cls");
            displayMenu();

            return;

        }


        // Check for 'B' key (66 in ASCII)
        if (GetKeyState(66) & 0x8000) {
            // Navigate back if the collection_history is not empty
            if (!collection_path.empty()) {
                current_collection = collection_path.front();
                collection_path.pop_front();

                // Clear the console and redraw the menu after changing the collection
                system("cls");
                displayMenu();
            }

            return;

        }

        // Check for 'D' key (68 in ASCII)
        if (GetKeyState(68) & 0x8000) {
            //Delete Selected Item
            current_collection->getItems().erase(current_collection->getItems().begin() + menu_item);

            // Clear the console and redraw the menu after adding the new entry
            system("cls");
            displayMenu();
            return;

        }




        // Check for 'Q' key (81 in ASCII)
        if (GetKeyState(81) & 0x8000) {
            FileHandler::saveToFile(root_collection, "main.csv");
            running = false;
            system("cls");
            return;


        }

                

    }
};