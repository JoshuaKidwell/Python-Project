#include "interpreter.h"
#include "LexicalAnalyzer.h"
#include <map>
#include <string>
#include <iostream>

interpreter::interpreter()
{
	elseCheck = false;
}

void interpreter::loadAndRunTokens(std::vector<std::vector<std::pair<std::string, categoryType>>> tokens) {
	tokenLines = tokens;
	for (int i = 0; i < tokens.size(); i++) {
		interpretLine(tokens[i], 0, tokens[i].size(), i);
	}
}

int interpreter::lastParen(std::vector<std::pair<std::string, categoryType>> tokenLine, int s)
{
	int numOfLeft = 1;
	int numOfRight = 0;
	do {
		if (s + 1 < tokenLine.size()) {
			s++;
		}
		else {
			std::cout << "Error. No Right Parenthesis..." << std::endl;
			break;
		}
		if (tokenLine[s].second == categoryType::RIGHT_PAREN) {
			numOfRight++;
		}
		if (tokenLine[s].second == categoryType::LEFT_PAREN) {
			numOfLeft++;
		}
	} while (numOfLeft > numOfRight);
	return s;
}

int interpreter::lastParenPrint(std::vector<std::pair<std::string, categoryType>> tokenLine, int s)
{
	int numOfLeft = 1;
	int numOfRight = 0;
	do {
		if (s + 1 < tokenLine.size()) {
			s++;
		}
		else {
			std::cout << "Error. No Right Parenthesis..." << std::endl;
			break;
		}
		if (tokenLine[s].second == categoryType::RIGHT_PAREN) {
			numOfRight++;
		}
		if (tokenLine[s].second == categoryType::LEFT_PAREN) {
			numOfLeft++;
		}
		if (tokenLine[s].second == categoryType::COMMA && (numOfRight + 1 == numOfLeft)) {
			break;
		}
	} while (numOfLeft > numOfRight);
	return s;
}

int interpreter::lastIndent(int s, int lineNum) {
	do {
		lineNum++;
		if (lineNum == tokenLines.size()) {
			break;
		}
	} while (tokenLines[lineNum][s].second == categoryType::INDENT);
	return lineNum - 1;
}

int interpreter::lastColon(std::vector<std::pair<std::string, categoryType>> tokenLine, int s)
{
	do {
		if (s + 1 < tokenLine.size()) {
			s++;
		}
		else {
			std::cout << "Error. No Colon..." << std::endl;
			break;
		}
	} while (tokenLine[s].second != categoryType::COLON);
	return s;
}

std::pair<std::string, categoryType> interpreter::interpretLine(std::vector<std::pair<std::string, categoryType>> tokenLine, int s, int e, int lineNum)
{
	int start = s;
	if (tokenLine[s].second == categoryType::COMMENT) {
		//Nothing to do here
		return { "null", categoryType::UNKNOWN };
	}
	else if (tokenLine[s].second == categoryType::KEYWORD) {
		if (tokenLine[s].first == "print") {
			if (s + 1 < e && tokenLine[s + 1].second == categoryType::LEFT_PAREN) {
				s++;
				while (s + 1 < e && tokenLine[s].second != categoryType::RIGHT_PAREN) {
					s++;
					int end = lastParenPrint(tokenLine, s);
					std::cout << interpretLine(tokenLine, s, end, lineNum).first;
					s = end;
					if (tokenLine[s].second == categoryType::COMMA) {
						std::cout << " ";
					}
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "Error. No Left Parenthesis after print..." << std::endl;
			}
			return { "1", categoryType::NUMERIC_LITERAL };
		}
		else if (tokenLine[s].first == "input") {
			if (s + 2 < e && tokenLine[s + 1].second == categoryType::LEFT_PAREN) {
				s += 2;
				std::pair<std::string, categoryType> value = interpretLine(tokenLine, s, lastParen(tokenLine, s), lineNum);
				if (value.second == categoryType::STRING_LITERAL) {
					std::cout << value.first;
					std::string input;
					std::cin >> input;
					if (s + 1 < e && tokenLine[s + 1].second == categoryType::RIGHT_PAREN) {
						s++;
						return { input, categoryType::STRING_LITERAL };
					}
					else {
						std::cout << "Error. No right parenthesis on the input..." << std::endl;
					}
				}
				else {
					std::cout << "Error. Input does not contain string..." << std::endl;
				}
			}
		}
		else if (tokenLine[s].first == "int") {
			if (s + 2 < e && tokenLine[s + 1].second == categoryType::LEFT_PAREN) {
				s += 2;
				std::pair<std::string, categoryType> value = interpretLine(tokenLine, s, lastParen(tokenLine, s), lineNum);
				if (value.second == categoryType::STRING_LITERAL && stoi(value.first)) {
					return { value.first, categoryType::NUMERIC_LITERAL };
				}
				else {
					std::cout << "Error. Not a string into int()..." << std::endl;
				}
			}
		}
		else if (tokenLine[s].second == categoryType::KEYWORD) {
			if (tokenLine[s].first == "if") {
				if (s + 1 < e) {
					s += 1;
					bool condition = stoi(interpretLine(tokenLine, s, lastColon(tokenLine, s), lineNum).first);
					if (condition) {
						int newLine = lineNum + 1;
						for (int k = newLine; k <= lastIndent(start, lineNum); k++) {
							interpretLine(tokenLines[k], start + 1, tokenLines[k].size(), newLine);
						}
						elseCheck = false;
					}
					else {
						elseCheck = true;
					}
				}
			}
			else if (tokenLine[s].first == "while") {
				if (s + 1 < e) {
					s++;
					bool condition = stoi(interpretLine(tokenLine, s, lastColon(tokenLine, s), lineNum).first);
					while (condition) {
						int newLine = lineNum + 1;
						for (int k = newLine; k <= lastIndent(start, lineNum); k++) {
							interpretLine(tokenLines[k], start + 1, tokenLines[k].size(), newLine);
						}
						condition = stoi(interpretLine(tokenLine, s, lastColon(tokenLine, s), lineNum).first);
					}
				}	
			}
			else if (tokenLine[s].first == "else") {
				if (s + 1 < e) {
					s += 1;
					if (elseCheck) {
						int newLine = lineNum + 1;
						for (int k = newLine; k <= lastIndent(start, lineNum); k++) {
							interpretLine(tokenLines[k], start + 1, tokenLines[k].size(), newLine);
						}
						elseCheck = false;
					}
				}
			}
			else if (tokenLine[s].first == "elif") {
				if (s + 1 < e) {
					s += 1;
					bool condition = stoi(interpretLine(tokenLine, s, lastColon(tokenLine, s), lineNum).first);
					if (condition && elseCheck) {
						int newLine = lineNum + 1;
						for (int k = newLine; k <= lastIndent(start, lineNum); k++) {
							interpretLine(tokenLines[k], start + 1, tokenLines[k].size(), newLine);
						}
						elseCheck = false;
					}
					else {
						elseCheck = true;
					}
				}
			}
		}
	}
	else if (tokenLine[s].second == categoryType::INDENT) {
		return { "null", categoryType::UNKNOWN };
	}
	else if (tokenLine[s].second == categoryType::STRING_LITERAL) {
		return { tokenLine[s].first.substr(1, tokenLine[s].first.length() - 2), categoryType::STRING_LITERAL };
	}
	else if (tokenLine[s].second == categoryType::NUMERIC_LITERAL) {
		std::string equation = "";
		for (s; s < e; s++) {
			equation = equation + tokenLine[s].first;
		}
		int value = evaluator.solveEquation(equation);
		return { std::to_string(value), categoryType::NUMERIC_LITERAL };
	}
	else if (tokenLine[s].second == categoryType::IDENTIFIER) {
		if (s + 2 < e && tokenLine[s + 1].second == categoryType::ASSIGNMENT_OP) {
			std::pair<std::string, categoryType> value = interpretLine(tokenLine, s + 2, e, lineNum);
			evaluator.addVariable(tokenLine[s].first, value.first);
			return { value.first, categoryType::NUMERIC_LITERAL };
		}
		else {
			std::string equation = "";
			for (s; s < e; s++) {
				equation = equation + tokenLine[s].first;
			}
			int value = evaluator.solveEquation(equation);
			return { std::to_string(value), categoryType::NUMERIC_LITERAL };
		}
	}


	return { "null", categoryType::UNKNOWN };
}

void interpreter::showVariables()
{
	std::map<std::string, std::string> var = evaluator.getSymbolMap();
	std::cout << "****** Variables *******" << std::endl;
	for (std::map<std::string, std::string>::iterator it = var.begin(); it != var.end(); it++) {
		std::cout << it->first << " = " << it->second << std::endl;
	}
}
