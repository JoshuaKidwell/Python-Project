#ifndef EXP_EVALUATOR
#define EXP_EVALUATOR

#include "LexicalAnalyzer.h"
#include <map>
#include <string>

class expEvaluator {
private:
	typedef std::map<std::string, std::string> symbolTableType;
	symbolTableType symbolTable;
	LexicalAnalyzer lexAnalyzer;
public:
	expEvaluator();
	int solveEquation(std::string equation);
	std::vector<std::pair<std::string, categoryType>> readIntoPostfix(std::string expression);
	int evaluatePostfix(std::vector<std::pair<std::string, categoryType>> postfixVect);
	int checkPresidence(std::pair<std::string, categoryType> symbol);
	void addVariable(std::string symbol, std::string value);
	symbolTableType getSymbolMap();
};

#endif

