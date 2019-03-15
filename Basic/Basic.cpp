/*
 * File: Basic.cpp
 * ---------------
 * Name: 517021910799
 * Section: Wang Yunchao
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include<set>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cout << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

bool isCmd(string line)//CHECK IF IS CMD
{
    static set<string> cmd{ "RUN", "LIST", "CLEAR", "QUIT", "HELP" };
    return cmd.count(line);
}

void command(string line, Program & program, EvalState & state)//EXECUTE COMMAND
{
    if (line == "RUN") {
        state.end = false;
        auto i = program.getFirstLineNumber();
        while (i != -1)//UNTIL EDGE
        {
            program.getParsedStatement(i)->execute(state);
            if (state.end)break;
            if (state.change)//NEED TO REDIRECT
            {
                i = state.changeTo;
                if (program.getSourceLine(i) == "")error("LINE NUMBER ERROR");
                state.change = false;
                continue;
            }
            i = program.getNextLineNumber(i);
        }
    }
    else if (line == "LIST")//PRINT ALL LINES
        for (auto i = program.getFirstLineNumber(); i != -1; i = program.getNextLineNumber(i))
        {
            cout << program.getSourceLine(i) << endl;
        }
    else if (line == "CLEAR")
    {
        program.clear();
        state.clear();
    }
    else if (line == "QUIT")
        exit(0);
    else if (line == "HELP")
        cout << "5 CAN TELL YOU";//WU KE FENG GAO
}

int isLine(string& line)//FIND AND ERASE LINENUMBER
{
    auto br = line.find(' ');
    auto num = line.substr(0, br);
    for (int i = 0; i < num.size(); i++)
    {
        if (!isdigit(num[i])) return -1;
    }
    line.erase(0, br);
    return atoi(num.c_str());
}

void processLine(string line, Program & program, EvalState & state) {
    string orgline = line;
    int lineNumber;
    string op;
    //COMMAND
    if (isCmd(line))
        command(line, program, state);
    //EXECUTE IMM
    else if ((lineNumber = isLine(line)) == -1)
    {
        auto stt=parseStt(line);
        stt->execute(state);
        delete stt;
    }
    //LINE
    else if(lineNumber!=-1)
    {
        if (line.empty()) program.removeSourceLine(lineNumber);
        else {
            program.addSourceLine(lineNumber, orgline);
            program.setParsedStatement(lineNumber, parseStt(line));
        }
    }
}
