/*
This program implements a advanced expression calculator.
Edited by Zhu Wenjie in 9/28/2018
Input from input.txt; output to output.txt.
The grammar for input is:

Statement:
Expression
Print

Print:
;

Expression:
Term
Expression + Term
Expression – Term

Term:
HighOrder_Term
Term * HighOrder_Term
Term / HighOrder_Term
Term % HighOrder_Term

HighOrder_Term:
TopOrder
-TopOrder
+TopOrder

TopOrder_Term:
Primary
Primary!

Primary:
ANS
Number
( Expression )
Number:
floating-point-literal
Input comes from fin through the Token_stream called ts.
*/

#include<iostream>
#include<cmath>
#include<string>
#include<fstream>
#include<stdexcept>
#include<memory>
using namespace std;

inline bool isInt(double a)
{
    return int(a) == a;
}

inline void error(const string& s)
{
    throw runtime_error(s);
}
//------------------------------------------------------------------------------

// Token holds operators, operands and ANS for last result

struct Token {
    char kind;
    double value;
    string name;
    Token(char ch) :kind(ch), value(0) { } // Operator
    Token(char ch, double val) :kind(ch), value(val) { } // Operand
    Token(char ch, string n) :kind(ch), name(n) { } // Signal
};

// Stream of tokens

ifstream fin("input.txt");
ofstream fout("output.txt");

class Token_stream {
    bool full;
    Token buffer; // Holds a symbol that wasn't used by specific function
public:
    Token_stream() :full(false), buffer(0) { }

    Token get();
    void unget(Token t) { buffer = t; full = true; } // Returns symbol to buffer. If 'full' = true, get() will use symbol from buffer

    void ignore(char); // Ignores all characters before specific symbol
};

double last_res = 0;//Restore the last result
const char ans = 'A'; // ANS is used for restore the last result
const char print = ';'; // Prints the result of calculation
const char number = '8'; // Indicates that this is a number

int leftnum = 0;//The number of ( which has no ) to match it
Token Token_stream::get() // Get characters from fin
{
    if (full) { full = false; return buffer; } // If buffer is not empty, return value from it
    char ch; // Otherwise start looking for characters in input stream
    fin>>ch;
    switch (ch) {
    case '(':
        leftnum++;
        return Token(ch);
    case ')':
        if (leftnum == 0)error("error");
        leftnum--;
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case ';':
    case '!':
        return Token(ch); // If it is an operator or comma then return it
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        fin.unget(); // If it is a number then return it to the stream
        double val;
        fin >> val; // Read the complete value
        return Token(number, val);
    }
    default:
        if (fin&&isalpha(ch)) { // If it is a letter
            string s;
            s += ch; // Put the letter in 's' because it has been already read
            while (fin.get(ch) && isalpha(ch) )s += ch; // Continue to read in 's'
            fin.unget(); // Return the character into the stream
            if (s == "ANS")
                return Token(ans);	// Evaluate the former result
        }
        error("error");
    }
}

void Token_stream::ignore(char c) // Ignores all characters before specific symbol
{
    if (full && c == buffer.kind) { // If 'c' is already in buffer then just discard it
        full = false;
        return;
    }
    full = false; // Otherwise discard value in buffer and look for it further
    char ch;
    while (fin >> ch) // Read all values before 'c'
        if (ch == c) return; // Return when 'c' is found
}

Token_stream ts; // Holds all Tokens
double expression();

double primary() // Processes semicolons, numbers and returns variables
{
        Token t = ts.get();
        switch (t.kind) {
        case ans: // Calculate  the last result
        {
            return last_res;
        }
        case '(':
        {	double d = expression(); // Perform calculations in semicolons
        t = ts.get(); // Get a ')' closing character
        if (t.kind != ')') { ts.unget(t); error("error"); } // If there wasn't any ')' return an error and recover the stream
        return d;
        }
        case number: // If Token is a number
            return t.value; // Return the number
        default:
            ts.unget(t);
            error("error");  // Return an error if an inappropriate character was provided
        } 
}

double toporder_term() // Performs '!'
{
    double left = primary(); // Get a number
    while (true) {
        Token t = ts.get(); // Get a new character
        switch (t.kind) {
        case '!': // Factorial
        {
            if (left == 0)return 1;
            if (left < 0 ||!isInt(left))error("error");
            double x = left;
            for (int i = 1; i < left; i++) { // Get a multiplication of all numbers before x (including x)
                x *= i;
            }
            if (x == 0) left = 1;
            else left = x;
            break;
        }
        default:
            ts.unget(t); // If nothing was done return character to the stream
            return left; // Return new or unchanged value of 'left'
        }
    }
}


double highorder_term() // Performs '+', '-'
{
    double k = 1;
    while(true)
    {

        Token t = ts.get(); // Get a new character
        switch (t.kind) {
        case '+':
        {
            break;
        }
        case '-': // For negative digits
        {   
            k *= -1;
            break;
        }
        default:
            ts.unget(t); // If nothing was done return character to the stream
            return k*toporder_term(); // Return new or unchanged value of 'left'
        }
    }
}

double term() // Performs '*', '/',  '%'
{
    double left = highorder_term(); // Get a number
    while (true) {
        Token t = ts.get(); // Get a new character
        switch (t.kind) {
        case '*': // Multiplication
            left *= highorder_term();
            break;
        case '/': // Division
        {	
            double d = highorder_term();
        if (d == 0) error("error"); // Division by zero is prohibited
        left /= d;
        break;
        }
        case '%': // Modulo
        {
            double d = highorder_term();
            if (d == 0) error("error");
            else if(isInt(left)&&isInt(d))//Each operand should be integer
            left = fmod(left, d); // Use fmod to divide floating-point numbers with remainder
            else error("error");
            break;
        }
        default:
            ts.unget(t); // If nothing was done return character to the stream
            if (left == -0) return 0; // Change -0 to 0 when it was multiplied or divided by negative digit
            return left; // Return new or unchanged value of 'left'
        }

    }
}

double expression() // Performs '+' and '-' operations
{
    double left = term(); // Get the number or the result of calculations in term
    while (true) {
        Token t = ts.get();
        switch (t.kind) {
        case '+':
            left += term(); // Addition
            break;
        case '-':
            left -= term(); // Subtraction
            t.kind = 'n';
            break;
        default:
            ts.unget(t); // If nothing was done return character to the stream
            return left; // Return the new or unchanged value of 'left'
        }
    }
}

void clean_up_mess() // Ignores all characters before ';' if an exception was thrown
{
    ts.ignore(print);
}

void calculate() // Performs calculations
{
    while (fin) try {//Iterating until EOF
        leftnum = 0;//reset the condition
        Token t = ts.get(); // Get a new character
        while (t.kind == print) t = ts.get(); // Read all ';'
        ts.unget(t); // Return a character into the stream
        if(fin){
            last_res = expression();
        fout << last_res << endl; // Output the result
        }
    }
    catch (runtime_error& e) {
        if (fin) {//Forbid output when it has been EOF
            fout << e.what() << endl; // fout the error if the exception was thrown
            clean_up_mess(); // Ignores all characters till the ';'
        }
    }
}

int main()
{
    calculate(); // Performs calculations
    return 0;
}