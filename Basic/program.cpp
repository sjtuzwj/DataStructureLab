/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

void Program::clear() {
    lines.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    //USE TO CLEAR THE FOMER STATEMENT
    lines[lineNumber] = line;
    if(stts.count(lineNumber))
    stts.erase(lineNumber);
}

void Program::removeSourceLine(int lineNumber) {
    if (lines.count(lineNumber))
    {
        //USE TO CLEAR THE FOMER STATEMENT
        lines.erase(lineNumber);
        if (stts.count(lineNumber))
            stts.erase(lineNumber);
    }
}

string Program::getSourceLine(int lineNumber) {
    if (lines.count(lineNumber))return lines[lineNumber];
    else return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    //CHECK IF LINE EXIST
    if (!lines.count(lineNumber))error("LINE NUMBER ERROR");
    else {
        stts[lineNumber] = shared_ptr<Statement>(stmt);
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
    if (stts.count(lineNumber))return stts[lineNumber].get();
    else return nullptr; 
}

int Program::getFirstLineNumber() {
    //CHECK IF LINE EMPTY
    if (lines.size() == 0)return -1;
    auto beg = lines.begin();
    return beg->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto cur = lines.find(lineNumber);
    // BORDER DETECT
    if (cur == lines.end())return -1;
    cur++;
    if (cur == lines.end())return -1;
    return cur->first;
}
