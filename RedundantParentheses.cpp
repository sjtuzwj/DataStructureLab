#include<iostream>
#include<string>
#include<stack>
#include<cctype>
#include<memory>
using namespace std;
/*
*Three stages:
*I2P
*Two stack:
*One store operand(which is now (expr))
*One store operator
*P2T:
*Using normal scanning
*T2I:
*Recursive evaluate the expression
*
*First we use a variation of Dijkstra Evaluation Algorithm to transform an infix expr to postfix
*Then we transform postfix expr to infix and add parenthesis by building a binary tree 
*/

//judge if a char is operator
bool isOp(char op)
{
    switch (op)
    {
    case '+':case'-':case'*':case'/':
        return true;
    default:
        return false;
    }
}
//judge if an operator is associative
bool isAssociative(char op)
{
    switch (op)
    {
    case '+':case'*':
        return true;
    default:
        return false;
    }
}
//get order of operator
int order(char op)
{
    switch (op)
    {
    case '+':case '-':return 1;
    case '*':case '/': return 2;
    default:
        return 0;
    }
}

//get original expression
string getExpr()
{
    string tmp;
    cin >> tmp;
    return tmp;
}

//Infix to postfix
string Dijkstra(string org)
{
    stack<char> oprs;//operators
                     //postfix expr
    string postfix;
    for (auto itr = org.begin(); itr != org.end(); ++itr)
    {
        char ch = *itr;
        switch (ch)
        {
        default://
                //1）operand:output
            postfix.push_back(ch);
            break;
        case '(':// 2）(: push
            oprs.push(ch);
            break;
        case')'://3）):pop until (, but not output (
            while (!oprs.empty())
            {
                char tmp = oprs.top();
                oprs.pop();
                if (tmp != '(')
                    postfix.push_back(tmp);
                else
                    break;
            }
            break;
        case'+':case'-':
        case'*':case'/':
            //4）any other operand, pop until operands with lower oder
            //Then push the operand
            while (!oprs.empty())
            {
                char tmp = oprs.top();
                if (order(tmp) >= order(ch))
                {
                    postfix.push_back(tmp);
                    oprs.pop();
                }
                else
                    break;
            }
            oprs.push(ch);
            break;
        }
    }
    while (!oprs.empty())
        //5) End pop all elements
    {
        postfix.push_back(oprs.top());
        oprs.pop();
    }
    return postfix;
}

//postfix string tree
struct Tree
{
    char ele;
    shared_ptr<Tree> Left;
    shared_ptr<Tree> Right;
    Tree(char e,shared_ptr<Tree> lhs,shared_ptr<Tree> rhs):ele(e),Left(lhs),Right(rhs){}
    Tree(char e) :ele(e) {}
    Tree() = default;
};

//add return value for pop method
shared_ptr<Tree> Pop(stack<shared_ptr<Tree>>&s)
{
    shared_ptr<Tree> tmp = s.top();
    s.pop();
    return  tmp;
}

//Postfix to Tree
shared_ptr<Tree> PostfixToTree(string org)
{
    stack<shared_ptr<Tree>> s;
    for (auto itr=org.begin(); itr != org.end(); itr++)
    {
        char ch = *itr;
        if (!isOp(ch))
            s.push(make_shared<Tree>(ch));
        else
        {
            auto right = Pop(s);
            auto left = Pop(s);
            s.push(make_shared<Tree>(ch,left,right));
        }
    }
    return Pop(s);
}

//Tree to infix
string Parse(shared_ptr<Tree> T)
{
        string strl;
        string strr;
        if (T)
        {
            strl = Parse(T->Left); //left tree
            strr = Parse(T->Right);//right tree
            if (T->Left)
            {
                //if order of left child node< parent node
                if (isOp(T->Left->ele) && order(T->ele) > order(T->Left->ele))
                    strl = '(' + strl + ')';
            }
            if (T->Right)
            {
                //if order of right child node<= parent node
                //and if the parent node operand is associative, <
                if (isOp(T->Right->ele) &&
                    (isAssociative(T->ele)&&order(T->ele)>order(T->Right->ele)||
                    !isAssociative(T->ele) && order(T->ele) >= order(T->Right->ele)))
                    strr = '(' + strr + ')';
            }
        }
        else return string();
        //return infix
        return strl+T->ele+strr;
}

//reduce parentheses
string reduce(string expr)
{
    return Parse(PostfixToTree(Dijkstra(expr)));
}

int main()
{
    string expr = getExpr();
    cout << reduce(expr) << endl;
    return 0;
}