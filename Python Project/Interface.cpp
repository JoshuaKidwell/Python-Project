#include <iostream>
#include "interface.h"
#include "interpreter.h"
#include "expEvaluator.h"
#include <ctype.h>

Interface::Interface() {
	running = false;
	contStatement = false;
}

void Interface::startInterface() {
	running = true;
	std::cout << "PySUB Interpreter 1.0 on Windows (August 2023)" << std::endl;
	std::cout << "Enter program lines or read(<filename>.py) at command line interface" << std::endl;
	std::cout << "Type 'help' for more information or 'quit' to exit" << std::endl;
}

void Interface::clear() {
	programCode.clear();
	std::cout << "Program cleared." << std::endl;
}

void Interface::quit() {
	std::cout << "Exiting PySUB Interpreter 1.0..." << std::endl;
	running = false;
}

void Interface::solve(std::string equation) {
	std::cout << evaluator.solveEquation(equation) << std::endl;
}

void Interface::run()
{
	interp.loadAndRunTokens(lexAnalyzer.tokens());
	std::cout << std::endl;
}

void Interface::checkCommand(std::vector<std::string> command) {
	if (command[0] == "help") {
		if (command[1] == "") {
			help();
		}
		else {
			help(command[1]);
		}
	}
	else if (command[0] == "read") {
		read(command[1]);
	}
	else if (command[0] == "show") {
		if (command[1] == "") {
			show();
		}
		else {
			show(command[1]);
		}
	}
	else if (command[0] == "quit") {
		quit();
	}
	else if (command[0] == "clear") {
		clear();
	}
	else if (command[0] == "run") {
		run();
	}
	else if (command[2] == ":") {
		interfaceCode.clear();
		interfaceCode = { command[0] };
		std::string code = "null";
		while (code != "") {
			std::cout << "...    ";
			getline(std::cin, code);
			interfaceCode.push_back('\t' + code);
		}
		interfaceCode.pop_back();
		lexAnalyzer.readToTokenType(interfaceCode);
		consolInterp.loadAndRunTokens(lexAnalyzer.tokens());
	}
	else {
		programType code;
		if (command[1] == "") {
			code = { command[0] };
		}
		else {
			code = { command[0] + "(" + command[1] + ")" };
		}
		lexAnalyzer.readToTokenType(code);
		bool onlyDigit = true;
		for (int i = 0; i < lexAnalyzer.tokens()[0].size(); i++) {
			if (!(lexAnalyzer.tokens()[0][i].second == categoryType::NUMERIC_LITERAL || lexAnalyzer.tokens()[0][i].second == categoryType::IDENTIFIER || lexAnalyzer.tokens()[0][i].second == categoryType::LOGICAL_OP || lexAnalyzer.tokens()[0][i].second == categoryType::ARITH_OP)) {
				onlyDigit = false;
			}
		}
		if (onlyDigit) {
			std::cout << consolInterp.interpretLine(lexAnalyzer.tokens()[0], 0, lexAnalyzer.tokens()[0].size(), 0).first << std::endl;
		} else 
			consolInterp.interpretLine(lexAnalyzer.tokens()[0], 0, lexAnalyzer.tokens()[0].size(), 0).first;
	}
}

std::vector<std::string> Interface::readinCommand() {
	std::string commandtxt;
	std::vector<std::string> content = {"", "" , ""};

	getline(std::cin, commandtxt);
	std::string command = "";
	int i = 0;
	if (commandtxt.length() > 0) {
			while (commandtxt.at(i) != '(' && i < (commandtxt.length() - 1)) {
				command = command + commandtxt.at(i);
				i++;
			}
			if (commandtxt.at(i) != '(')
				command = command + commandtxt.at(i);
			content[0] = command;
			command = "";
			if (commandtxt.at(i) == '(') {
				i++;
				while (commandtxt.at(i) != ')') {
					command = command + commandtxt.at(i);
					i++;
				}
				content[1] = command;
			}
	} 
	else {
		content[0] = "null";
		std::cout << std::endl;
	}
	if (commandtxt.at(commandtxt.length() - 1) == ':') {
		content[2] = ":";
	}

	return content;
}

void Interface::read(std::string name) {

	std::ifstream file;
	file.open(name);
	if (file) {
		std::cout << "File loaded." << std::endl;
	}
	else {
		std::cout << "File did not load..." << std::endl;
	}
	int count = 0;
	std::string line;
	while (std::getline(file, line)) {
		programCode.push_back(line);
	}
	file.close();

	lexAnalyzer.readToTokenType(programCode);
}

void Interface::show() {
	for (auto line : programCode) {
		std::cout << ">>> " << line << std::endl;
	}
}

void Interface::show(std::string name) {
	if (name == "tokens") {
		lexAnalyzer.show();
	}
	else if (name == "variables") {
		interp.showVariables();
	}
	else {
		std::cout << "Error... " << name << " is not a showable term" << std::endl;
	}
}

void Interface::help() {
	std::cout << "Welcome to the help utility!" << std::endl;
	std::cout << "* To exit and return to the interpreter, type 'exit'" << std::endl;
	std::cout << "* To get a list of commands, type 'commands'" << std::endl;
	std::cout << "* To get a description of any command, just type the command at the help> promt" << std::endl;

	bool helpRunning = true;
	std::string response;
	std::string word = "";
	while (helpRunning) {
		std::cout << "help> ";
		std::vector<std::string> command;
		command = readinCommand();

		help(command[0]);

		if (command[0] == "exit") {
			helpRunning = false;
			std::cout << std::endl;
		}
	}
}

void Interface::help(std::string command) {
	if (command == "commands") {
		std::cout << "Below is a list of commands.Enter any commmand at the prompt to get more help \n \nclear\thelp\tquit\nread\tshow\n" << std::endl;
		std::cout << "NOTE:: All commands can also be entered as functions: \n\nclear()\thelp()\tquit()\nread()\tshow()\n\n";
	}
	if (command == "clear") {
		std::cout << "This command clears all lines read into the interpreter." << std::endl;
	}
	if (command == "help") {
		std::cout << "This command gives you information each useable command." << std::endl;
	}
	if (command == "show") {
		std::cout << "This command displays the file that has been read into the system." << std::endl;
	}
	if (command == "read") {
		std::cout << "This command reads in a file using the file name such as read(filename.py)." << std::endl;
	}
	if (command == "quit") {
		std::cout << "This command exits the Interpreter." << std::endl;
	}
}

void Interface::exitInterface() {
	std::cout << "PySUB Interpreter Closed" << std::endl;
	running = false;
}

bool Interface::isRunning() {
	return running;
}