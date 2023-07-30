
#include <InteractiveConsole.h>
#include <FileManager.h>
#include <iostream>

int main() {

	// std::shared_ptr<Collection> root = std::make_unique<Collection>("Root", -1);
	// root->createEntry("First Entry");
	// root->createEntry("Second Entry");
	// root->createEntry("Third Entry");
	// root->createEntry("4 Entry");
	// root->createEntry("5 Entry");
	// root->createEntry("6 Entry");
	// root->createEntry("7 Entry");
	// root->createEntry("8 Entry");
	// root->createEntry("9 Entry");
	// root->createEntry("10 Entry");
	// root->createSubCollection("Sub1");
	// FileHandler::saveToFile(root, "main.csv");


	//Reconstruct hierarchy from csv
	std::shared_ptr<Collection> collection = FileHandler::readFromFile("main.csv");
    auto root = std::static_pointer_cast<Collection>(collection->getItems()[0]);

	//Create the CLI interface
    InteractiveConsole console(root);
    console.run();

    return 0;
}
