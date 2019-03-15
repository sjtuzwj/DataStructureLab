/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include<vector>
#include<string>
#include<sstream>
#include "evalstate.h"
#include"parser.h"
#include "exp.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

};



class SequentialStatement:public Statement {
    //show the type of statement
    string type;
    //store some signal
    vector<string> op;
    //store some expr
    vector<Expression*> ex;
public:

    SequentialStatement(string token, const  vector<string>& ops, const vector<Expression*>& exs):type(token),op(ops),ex(exs){}


    virtual ~SequentialStatement() = default;

    /*
    * Method: execute
    * Usage: stmt->execute(state);
    * ----------------------------
    * This method executes a BASIC statement.  Each of the subclasses
    * defines its own execute method that implements the necessary
    * operations.  As was true for the expression evaluator, this
    * method takes an EvalState object for looking up variables or
    * controlling the operation of the interpreter.
    */

    void execute(EvalState & state);

};


/*
 * Class: ControlStatement
 * ----------------
 * This class is used to represent a controlstatement in a program.
 */

class ControlStatement :public Statement {
    //show the type of statement
    string type;
    //store some signal
    vector<string> op;
    //store some expr
    vector<Expression*> ex;
public:

    ControlStatement(string token, const vector<string>& ops, const vector<Expression*>& exs) :type(token), op(ops), ex(exs) {}
    virtual ~ControlStatement() = default;

    /*
    * Method: execute
    * Usage: stmt->execute(state);
    * ----------------------------
    * This method executes a BASIC statement.  Each of the subclasses
    * defines its own execute method that implements the necessary
    * operations.  As was true for the expression evaluator, this
    * method takes an EvalState object for looking up variables or
    * controlling the operation of the interpreter.
    */

    void execute(EvalState & state);
};
#endif
