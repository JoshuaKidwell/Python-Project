#include "LexicalAnalyzer.h"
#include <iostream>
#include <string>
#include <cctype>

LexicalAnalyzer::LexicalAnalyzer() {
	keywordList = { "print", "def", "return", "if", "input", "int", "while", "elif", "else"};
	logicalOpList = { "or", "not", "and" };
}

void LexicalAnalyzer::readToTokenType(std::vector<std::string> programCode) {
	tokenInfo.clear();
	std::vector<char> line;
	for (int i = 0; i < programCode.size(); i++) {
		line.clear();
		for (std::string::iterator it = programCode[i].begin(); it != programCode[i].end(); it++) {
			line.push_back(*it);
		}
		tokenInfo.push_back(readLineToTokenType(line));
	}
}

LexicalAnalyzer::tokenLineType LexicalAnalyzer::readLineToTokenType(std::vector<char> line) {
	tokenLineType tokenLine;
	pairType pair;
	std::string token = "";
	int i = 0;
	while (i < line.size()) {
		if (isdigit(line[i])) {
			while (isdigit(line[i])) {
				token = token + line[i];
				if (i + 1 < line.size()) {
					i++;
				}
				else {
					i++;
					break;
				}
			}
			pair.first = token;
			pair.second = categoryType::NUMERIC_LITERAL;
			tokenLine.push_back(pair);
		}
		else if (isalpha(line[i])) {
			while (isalpha(line[i])) {
				token = token + line[i];
				if (i + 1 < line.size()) {
					i++;
				}
				else {
					i++;
					break;
				}
			}
			pair.first = token;
			bool isKeyword = false;
			bool isLogicalOp = false;
			for (int j = 0; j < keywordList.size(); j++) {
				if (keywordList[j] == token) {
					isKeyword = true;
				}
			}
			for (int j = 0; j < logicalOpList.size(); j++) {
				if (logicalOpList[j] == token) {
					isLogicalOp = true;
				}
			}
			if (isKeyword) {
				pair.second = categoryType::KEYWORD;
			}
			else if (isLogicalOp) {
				pair.second = categoryType::LOGICAL_OP;
			}
			else {
				pair.second = categoryType::IDENTIFIER;
			}
			tokenLine.push_back(pair);
		}
		else if (line[i] == '\'') {
			token = token + line[i];
			i++;
			while (line[i] != '\'') {
				token = token + line[i];
				i++;
			}
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::STRING_LITERAL;
			tokenLine.push_back(pair);
		}
		else if (line[i] == '\"') {
			token = token + line[i];
			i++;
			while (line[i] != '\"') {
				token = token + line[i];
				i++;
			}
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::STRING_LITERAL;
			tokenLine.push_back(pair);
		}
		else if (line[i] == '(') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::LEFT_PAREN;
			tokenLine.push_back(pair);
		}
		else if (line[i] == ')') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::RIGHT_PAREN;
			tokenLine.push_back(pair);
		}
		else if (line[i] == ',') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::COMMA;
			tokenLine.push_back(pair);
		}
		else if (line[i] == ':') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::COLON;
			tokenLine.push_back(pair);
		}
		else if (line[i] == ':') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::COLON;
			tokenLine.push_back(pair);
		}
		else if (line[i] == '#') {
			while (i < line.size()) {
				token = token + line[i];
				i++;
			}
			pair.first = token;
			pair.second = categoryType::COMMENT;
			tokenLine.push_back(pair);
		}
		else if (line[i] == '\t' && i == 0) {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::INDENT;
			tokenLine.push_back(pair);
		}
		else if (line[i] == ' ') {
			i++;
		}
		else if (line[i] == '+' || line[i] == '*' || line[i] == '-' || line[i] == '/' || line[i] == '%') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::ARITH_OP;
			tokenLine.push_back(pair);
		}
		else if ((i + 1 < line.size()) && ((line[i] == '=' && line[i + 1] == '=') || (line[i] == '!' && line[i + 1] == '=') || (line[i] == '<' && line[i + 1] == '=') || (line[i] == '>' && line[i + 1] == '='))) {
			token = token + line[i];
			i++;
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::RELATIONAL_OP;
			tokenLine.push_back(pair);
		}
		else if (line[i] == '>' || line[i] == '<') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::RELATIONAL_OP;
			tokenLine.push_back(pair);
		}
		else if (line[i] == '=') {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::ASSIGNMENT_OP;
			tokenLine.push_back(pair);
		}
		else {
			token = token + line[i];
			i++;
			pair.first = token;
			pair.second = categoryType::UNKNOWN;
			tokenLine.push_back(pair);
		}
		token = "";
	}
	return tokenLine;
}

LexicalAnalyzer::tokenType LexicalAnalyzer::tokens()
{
	return tokenInfo;
}

void LexicalAnalyzer::show() {
	std::cout << "***** TOKEN INFORMATION *****" << std::endl;
	for (int i = 0; i < tokenInfo.size(); i++) {
		std::cout << "Line #" << i << ":" << std::endl;
		for (int j = 0; j < tokenInfo[i].size(); j++) {
			std::cout << "Token[" << j << "]:  " << tokenInfo[i][j].first << " - ";
			switch (tokenInfo[i][j].second) {
			case categoryType::KEYWORD:
				std::cout << "KEYWORD";
				break;
			case categoryType::IDENTIFIER:
				std::cout << "IDENTIFIER";
				break;
			case categoryType::STRING_LITERAL:
				std::cout << "STRING_LITERAL";
				break;
			case categoryType::NUMERIC_LITERAL:
				std::cout << "NUMERIC_LITERAL";
				break;
			case categoryType::ASSIGNMENT_OP:
				std::cout << "ASSIGNMENT_OP";
				break;
			case categoryType::ARITH_OP:
				std::cout << "ARITH_OP";
				break;
			case categoryType::LOGICAL_OP:
				std::cout << "LOGICAL_OP";
				break;
			case categoryType::RELATIONAL_OP:
				std::cout << "RELATIONAL_OP";
				break;
			case categoryType::LEFT_PAREN:
				std::cout << "LEFT_PAREN";
				break;
			case categoryType::RIGHT_PAREN:
				std::cout << "RIGHT_PAREN";
				break;
			case categoryType::COLON:
				std::cout << "COLON";
				break;
			case categoryType::COMMA:
				std::cout << "COMMA";
				break;
			case categoryType::COMMENT:
				std::cout << "COMMENT";
				break;
			case categoryType::INDENT:
				std::cout << "INDENT";
				break;
			case categoryType::UNKNOWN:
				std::cout << "UNKNOWN";
				break;
			}
			std::cout << std::endl;
		}
		std::cout << "-------------------------------------------------------------------" << std::endl;
	}
	std::cout << std::endl;
}