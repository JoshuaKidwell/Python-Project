#include "expEvaluator.h"
#include <vector>
#include <iostream>
#include "LexicalAnalyzer.h"

expEvaluator::expEvaluator() {
}

int expEvaluator::solveEquation(std::string equation)
{
	return evaluatePostfix(readIntoPostfix(equation));
}

std::vector<std::pair<std::string, categoryType>> expEvaluator::readIntoPostfix(std::string expression) {
	std::vector<char> equation(expression.begin(), expression.end());
	std::vector<std::pair<std::string, categoryType>> infixVect = lexAnalyzer.readLineToTokenType(equation);
	std::vector<std::pair<std::string, categoryType>> stackVect;
	std::vector<std::pair<std::string, categoryType>> postfixVect;

	for (int i = 0; i < infixVect.size(); i++) {
		if (infixVect[i].second == categoryType::NUMERIC_LITERAL) {
			postfixVect.push_back(infixVect[i]);
		}
		else if (infixVect[i].second == categoryType::IDENTIFIER) {
			if (symbolTable.find(infixVect[i].first) != symbolTable.end()) {
				std::pair<std::string, categoryType> x = {symbolTable[infixVect[i].first], categoryType::NUMERIC_LITERAL};
				postfixVect.push_back(x);
			}
			else {
				std::cout << "Error. Variable Value not found..." << std::endl;
			}
		}
		else {
			if (infixVect[i].first == "(") {
				stackVect.push_back(infixVect[i]);
			}
			else if (infixVect[i].first == ")") {
				int j;
				if (stackVect.size() != 0) {
					j = stackVect.size() - 1;
					while (stackVect[j].first != "(") {
						std::pair<std::string, categoryType> y = stackVect[j];
						stackVect.pop_back();
						postfixVect.push_back(y);
						j--;
					}
					stackVect.pop_back();
				}
				else {
					std::cout << "Error with parentheses... " << std::endl;
				}
			}
			else if (infixVect[i].second == categoryType::ARITH_OP || infixVect[i].second == categoryType::LOGICAL_OP || infixVect[i].second == categoryType::RELATIONAL_OP) {
				int j;
				if (stackVect.size() != 0) {
					j = stackVect.size() - 1;
				}
				while (stackVect.size() > 0 && (checkPresidence(stackVect[j]) >= checkPresidence(infixVect[i]) && stackVect[j].first != "(")) {
					std::pair<std::string, categoryType> y = stackVect[j];
					stackVect.pop_back();
					postfixVect.push_back(y);
					j--;
				}
				stackVect.push_back(infixVect[i]);
			}
		}
	}
	for (int j = stackVect.size() - 1; j >= 0; j--) {
		std::pair<std::string, categoryType> y = stackVect[j];
		stackVect.pop_back();
		postfixVect.push_back(y);
	}

	return postfixVect;
}

int expEvaluator::evaluatePostfix(std::vector<std::pair<std::string, categoryType>> postfixVect)
{
	bool valueIsUndefined = true;
	std::vector<std::pair<std::string, categoryType>> stackVect;
	for (int i = 0; i < postfixVect.size(); i++) {
		if (postfixVect[i].second == categoryType::NUMERIC_LITERAL) {
			stackVect.push_back(postfixVect[i]);
		}
		else if (postfixVect[i].first == "not") {
			int j = stackVect.size() - 1;
			int a = stoi(stackVect[j].first);
			stackVect.pop_back();
			std::pair<std::string, categoryType> x = { std::to_string(!a), categoryType::NUMERIC_LITERAL };
			stackVect.push_back(x);
		}
		else if (postfixVect[i].second == categoryType::ARITH_OP || postfixVect[i].second == categoryType::LOGICAL_OP || postfixVect[i].second == categoryType::RELATIONAL_OP) {
			int j = stackVect.size() - 1;
			int b = stoi(stackVect[j].first);
			int a = stoi(stackVect[j - 1].first);
			stackVect.pop_back();
			stackVect.pop_back();
			if (postfixVect[i].first == "or") {
				std::pair<std::string, categoryType> x = { std::to_string(a or b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "and") {
				std::pair<std::string, categoryType> x = { std::to_string(a and b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "<") {
				std::pair<std::string, categoryType> x = { std::to_string(a < b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == ">") {
				std::pair<std::string, categoryType> x = { std::to_string(a > b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "<=") {
				std::pair<std::string, categoryType> x = { std::to_string(a <= b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == ">=") {
				std::pair<std::string, categoryType> x = { std::to_string(a >= b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "!=") {
				std::pair<std::string, categoryType> x = { std::to_string(a != b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "==") {
				std::pair<std::string, categoryType> x = { std::to_string(a == b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "+") {
				std::pair<std::string, categoryType> x = { std::to_string(a + b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "-") {
				std::pair<std::string, categoryType> x = { std::to_string(a - b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "*") {
				std::pair<std::string, categoryType> x = { std::to_string(a * b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "/") {
				std::pair<std::string, categoryType> x = { std::to_string(a / b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
			else if (postfixVect[i].first == "%") {
				std::pair<std::string, categoryType> x = { std::to_string(a % b), categoryType::NUMERIC_LITERAL };
				stackVect.push_back(x);
			}
		}
	}
	return stoi(stackVect[0].first);
}

int expEvaluator::checkPresidence(std::pair<std::string, categoryType> symbol)
{
	if (symbol.first == "or") {
		return 0;
	}
	else if (symbol.first == "and") {
		return 1;
	}
	else if (symbol.first == "not") {
		return 2;
	}
	else if (symbol.second == categoryType::RELATIONAL_OP) {
		return 3;
	}
	else if (symbol.first == "+" || symbol.first == "-") {
		return 4;
	}
	else if (symbol.first == "*" || symbol.first == "/" || symbol.first == "%") {
		return 5;
	}
}

void expEvaluator::addVariable(std::string symbol, std::string value)
{
	if (symbolTable.find(symbol) == symbolTable.end())
	{
		symbolTable.insert({ symbol, value });
	}
	else {
		symbolTable[symbol] = value;
	}
}

expEvaluator::symbolTableType expEvaluator::getSymbolMap()
{
	return symbolTable;
}
