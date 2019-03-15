/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"
#include "statement.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));
   if (token != "(") error("SYNTAX ERROR");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("SYNTAX ERROR");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}

Statement* parseStt(string line)//GENERATE STATEMENT ACCORDING TO THE GRAMMAR
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.setInput(line);
    string token = scanner.nextToken();
    //GRAMMAR LIKE THESE
    //REM ANY
    //END
    if (token == "REM"||token=="END")
        return new SequentialStatement(token, vector<string>(),vector<Expression*>());
    //LET VAR = EXPR
    else if (token == "LET")
    {
        string val = scanner.nextToken();
        string eq = scanner.nextToken();
        Expression *exp = parseExp(scanner);
        if (scanner.hasMoreTokens())error("SYNTAX ERROR");
        return new SequentialStatement(token, vector<string>{val}, vector<Expression*>{exp});
    }
    //PRINT EXPR
    else if (token == "PRINT")
    {
        Expression *exp = parseExp(scanner);
        if (scanner.hasMoreTokens())error("SYNTAX ERROR");
        return new SequentialStatement(token, vector<string>{}, vector<Expression*>{exp});
    }
    //INPUT VAR
    else if (token == "INPUT")
    {
        string val = scanner.nextToken();
        if (scanner.hasMoreTokens())error("SYNTAX ERROR");
        return new SequentialStatement(token, vector<string>{val}, vector<Expression*>());

    }
    //GOTO VAL
    else if (token == "GOTO")
    {
        string val = scanner.nextToken();
        if (scanner.hasMoreTokens())error("SYNTAX ERROR");
        return new ControlStatement(token, vector<string>{val}, vector<Expression*>());
    }
    //IF EXPR CMP EXPR THEN VAL
    else if (token == "IF")
    {
        Expression *exp1 = parseExp(scanner);
        string cmp = scanner.nextToken();//±‹√‚=∫≈±ªÕÃ√ª
        Expression *exp2 = parseExp(scanner);
        string then = scanner.nextToken();
        string val = scanner.nextToken();
        if (scanner.hasMoreTokens())error("SYNTAX ERROR");
        return new ControlStatement(token, vector<string>{cmp, val}, vector<Expression*>{exp1,exp2});
    }
}
