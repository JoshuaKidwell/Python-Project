#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expEvaluator.h"

class interpreter
{
public:
	interpreter();
	void loadAndRunTokens(std::vector<std::vector<std::pair<std::string, categoryType>>>);
	std::pair<std::string, categoryType> interpretLine(std::vector<std::pair<std::string, categoryType>>, int s, int e, int);
	void showVariables();
	int lastParen(std::vector<std::pair<std::string, categoryType>>, int s);
	int lastParenPrint(std::vector<std::pair<std::string, categoryType>>, int s);
	int lastIndent(int s, int lineNum);
	int lastColon(std::vector<std::pair<std::string, categoryType>>, int s);
	std::vector<std::vector<std::pair<std::string, categoryType>>> tokenLines;
private:
	expEvaluator evaluator;
	bool elseCheck;
};

#endif

