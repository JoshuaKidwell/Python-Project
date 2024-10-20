#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <vector>
#include <string>

enum class categoryType
{
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};

class LexicalAnalyzer {
private:
    typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
    typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
    typedef std::pair<std::string, categoryType> pairType;
    tokenType tokenInfo;
    std::vector<std::string> keywordList;
    std::vector<std::string> logicalOpList;

public:
    LexicalAnalyzer();
    void readToTokenType(std::vector<std::string> programCode);
    tokenLineType readLineToTokenType(std::vector<char> line);
    tokenType tokens();
    void show();
};

#endif

