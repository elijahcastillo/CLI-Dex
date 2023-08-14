
#include <InteractiveConsole.h>
#include <FileManager.h>
#include <iostream>

int main() {


	//Reconstruct hierarchy from csv
	std::shared_ptr<Collection> collection = FileHandler::readFromFile("CLI-Dex.csv");
    auto root = std::static_pointer_cast<Collection>(collection->getItems()[0]);

	//Create the CLI interface
    InteractiveConsole console(root);
    console.run();

    return 0;
}
