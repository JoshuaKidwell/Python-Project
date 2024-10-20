#ifndef INTERFACE_H
#define INTERFACE_H

#include "lexicalanalyzer.h"
#include "expEvaluator.h"
#include <vector>
#include <string>
#include <fstream>
#include "interpreter.h"

class Interface {
public:
	Interface();
	void startInterface();
	void exitInterface();
	bool isRunning();
	void show();
	void show(std::string name);
	void help();
	void help(std::string name);
	void read(std::string name);
	void clear();
	void checkCommand(std::vector<std::string> command);
	void quit();
	void solve(std::string equation);
	void run();
	std::vector<std::string> readinCommand();

private:
	typedef std::vector<std::string> programType;
	programType programCode;
	programType interfaceCode;
	bool running;
	LexicalAnalyzer lexAnalyzer;
	expEvaluator evaluator;
	interpreter interp;
	interpreter consolInterp;
	bool contStatement;
};

#endif
