/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include<set>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"
using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
   /* Empty */
}

EvalState::~EvalState() {
   /* Empty */
}

void EvalState::setValue(string var, int value) {//forbid using key word
    static set<string> ILLEGAL{"LIST", "QUIT", "LET","REM", "RUN", "LIST", "HELP", "QUIT", "CLEAR", "GOTO", "IF", "THEN", "END", "PRINT", "INPUT"};
    if (ILLEGAL.count(var))error("SYNTAX ERROR");
    symbolTable.put(var, value);
}

int EvalState::getValue(string var) {
  if(isDefined(var))//check if the var is defined
   return symbolTable.get(var);
  else error("VARIABLE NOT DEFINED");
}

bool EvalState::isDefined(string var) {
   return symbolTable.containsKey(var);
}
