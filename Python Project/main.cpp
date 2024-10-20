#include <iostream>
#include "interface.h"

int main()
{
	Interface pySubInterpreter;

	// Start the interface
	pySubInterpreter.startInterface();
	while (pySubInterpreter.isRunning()) {
		std::vector<std::string> command;
		std::cout << ">>> ";
		command = pySubInterpreter.readinCommand();
		pySubInterpreter.checkCommand(command);
	}

	return 0;
}
