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

#define BLACK "\033[30m"
#define LIGHT_RED "\033[91m"
#define LIGHT_GREEN "\033[92m"
#define LIGHT_YELLOW "\033[93m"
#define LIGHT_BLUE "\033[94m"
#define LIGHT_MAGENTA "\033[95m"
#define LIGHT_CYAN "\033[96m"
#define LIGHT_WHITE "\033[97m"
#define GRAY "\033[90m"
#define DARK_GRAY "\033[90;1m" // A slightly brighter shade of gray

// If you want bold versions of the existing colors
#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_WHITE "\033[1;37m"

// If you want underline versions of the existing colors
#define UNDERLINE_RED "\033[4;31m"
#define UNDERLINE_GREEN "\033[4;32m"
#define UNDERLINE_YELLOW "\033[4;33m"
#define UNDERLINE_BLUE "\033[4;34m"
#define UNDERLINE_MAGENTA "\033[4;35m"
#define UNDERLINE_CYAN "\033[4;36m"
#define UNDERLINE_WHITE "\033[4;37m"







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
        std::cout << BLUE << "===================================" << RESET;
        gotoXY(12, start_y - 3);
        std::cout << "   ";
        for (auto it = collection_path.rbegin(); it != collection_path.rend(); ++it) {
            std::cout <<  (*it)->getData() << " / ";
        }
        std::cout << current_collection->getData();
        gotoXY(12, start_y - 2);
        std::cout << BLUE << "===================================" << RESET;


        
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
        std::cout << BLUE  << "(N)" << CYAN << " Create New Entry" << RESET;

        start_y++;
        gotoXY(12, start_y);
        std::cout  << BLUE << "(C)" << CYAN << " Create New Collection"<< RESET;

        start_y++;
        gotoXY(12, start_y);
        std::cout  << BLUE << "(D)"<< CYAN << " Delete Selected"<< RESET;

        start_y++;
        gotoXY(12, start_y);
        std::cout << BLUE << "(B)"<< CYAN <<" Go Back"<< RESET;

        start_y++;
        gotoXY(12, start_y);
        std::cout  << BLUE << "(Q)"<< CYAN << " Quit & Save"<< RESET;



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
            FileHandler::saveToFile(root_collection, "CLI-Dex.csv");
            running = false;
            system("cls");
            return;


        }

                

    }
};