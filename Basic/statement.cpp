/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

void SequentialStatement::execute(EvalState & state)
{
    if (type == "REM")return;
    else if (type == "END") {//signal bit to end the program
        state.end = true; return;
    }
    else if (type == "LET")
    {
        state.setValue(op[0], ex[0]->eval(state));
        return;
    }
    else if (type == "PRINT")
    {
        cout << ex[0]->eval(state) << endl;
        return;
    }
    else if (type == "INPUT")//INPUT UNTIL THE INPUT IS LEGAL
    {
        cout << " ? ";
        string num;
        bool ac = false;
        while (!ac)
        {
            getline(cin, num);//FORBID INPUT EXPR
            ac = true;
            for (int i = 0; i < num.size(); i++)
            {
                if (!isdigit(num[i]) && !(i == 0 && num[i] == '-')) {
                    ac = false;
                    cout << "INVALID NUMBER" << endl;
                    cout << " ? ";
                    break;
                }
            }
        }
        state.setValue(op[0], atoi(num.c_str()));
        return;
    }
}
void ControlStatement::execute(EvalState & state)
{

    if (type == "GOTO")//Change control power by state signal
    {
        state.change = true;
        state.changeTo = atoi(op[0].c_str());
        return;
    }
    else if (type == "IF")//similar to goto but add a condition
    {
        auto lhs = ex[0]->eval(state);
        auto rhs = ex[1]->eval(state);
        bool cdt = false;
        if (op[0] == "<")cdt = lhs < rhs;
        else if (op[0] == ">")cdt = lhs > rhs;
        else if (op[0] == "=")cdt = (lhs == rhs);
        if (cdt)
        {
            state.change = true;
            state.changeTo = atoi(op[1].c_str());
        }
    }
}