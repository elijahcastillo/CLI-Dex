#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include "Collection.h"
#include "FileManager.h"



// class IC {


//     IC() {

//         int menu_y_start = 7
//         console = GetStdHandle(STD_OUTPUT_HANDLE);
//         gotoXY(1, 3);
//         std::cout << "Menu";
//         gotoXY(7, 7);
//         std::cout << "->";

//         menu_y = menu_y_start;
//     }

//     void run() {
//         while (running) {
//             displayMenu();
//             handleInput();
//         }
//         return menu_item;
//     }

// private:
//     HANDLE console;
//     COORD CursorPosition;
//     int menu_y = 0;
//     int menu_x = 7;
//     int menu_item = 0;
//     bool running = true;
//     s
// }

class InteractiveConsole {
public:
    InteractiveConsole(std::shared_ptr<Collection> root_collection) : current_collection(root_collection), root_collection(root_collection), running(true) {
        console = GetStdHandle(STD_OUTPUT_HANDLE);
        // gotoXY(1, 5);
        // std::cout << "Main Menu";
        gotoXY(10, 7);
        std::cout << "->";
    }



    void displayMenu() {

        // Clear the screen before displaying the menu
        // system("cls");

        int start_y = 7;
        gotoXY(12, start_y - 4);
        std::cout << "===================================";
        gotoXY(12, start_y - 3);
        std::cout << "  "<< current_collection->getData();
        gotoXY(12, start_y - 2);
        std::cout << "===================================";

        
        for (size_t i = 0; i < current_collection->getItems().size(); ++i) {
            gotoXY(12, start_y);
            std::cout << "[" << "x" << "] " << current_collection->getItems()[i]->getData();
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
        std::cout << "[N] Create New Entry";

        start_y++;
        gotoXY(12, start_y);
        std::cout << "[B] Go Back";

        start_y++;
        gotoXY(12, start_y);
        std::cout << "[Q] Quit";



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
    std::vector<std::string> menuItems;
    std::shared_ptr<Collection> root_collection;
    std::shared_ptr<Collection> current_collection;
    std::shared_ptr<Collection> prev_collection;




    void goToSubCollection(std::shared_ptr<Collection> sub_collection){
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


    if (GetAsyncKeyState(VK_DOWN)) {
        gotoXY(9, 7 + x);
        std::cout << "  ";
        x = (x + 1) % current_collection->getItems().size();
        gotoXY(9, 7 + x);
        std::cout << "->";
        menu_item = x;
        return;
    }

    if (GetAsyncKeyState(VK_UP)) {
        gotoXY(9, 7 + x);
        std::cout << "  ";
        x = (x + current_collection->getItems().size() - 1) % current_collection->getItems().size();
        gotoXY(9, 7 + x);
        std::cout << "->";
        menu_item = x;
        return;
    }

    if (GetAsyncKeyState(VK_RETURN)) {
        // gotoXY(20, 16);
        // std::cout << "You chose " << current_collection->getItems()[menu_item]->getData() << "...     ";
        if(current_collection->getItems()[menu_item]->getType() == TodoItem::ClassType::Collection){
            auto collection = std::static_pointer_cast<Collection>(current_collection->getItems()[menu_item]);
            prev_collection = current_collection;
            goToSubCollection(collection);

        }
    }


    // Check for 'N' key (78 in ASCII)
    if (GetAsyncKeyState(78) & 0x8000) {

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
    }


    // Check for 'B' key (66 in ASCII)
    if (GetAsyncKeyState(66) & 0x8000) {
        if(prev_collection != nullptr){
            goToSubCollection(prev_collection);
        }
    }

    // Check for 'Q' key (81 in ASCII)
    if (GetAsyncKeyState(81) & 0x8000) {
        FileHandler::saveToFile(root_collection, "main.csv");
        running = false;
        system("cls");

    }
}
};