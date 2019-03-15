/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Zhu Wenjie
 * Student ID:517021910799
 * This file is the starter project for the word ladder problem of Lab 2.
 * 
 * The project uses BFS as the core algorithm to find a shortest path between two words of the same size where on each step one letter will change
 * The project uses a series of hash table to build a group of child graph built classified by size from the whole word set 
 * Each map has its node in the form of pair<string,word>, the string will store the word itself, and the word store the min path and the neighbor
 * For each input, we will only build the child graph if not built and ignore others so that the problem is simplified.
 *
 * Input form:
 * first: you need to input the name of the text file that store the word set
 * then : just input the start word and the destination word repeatedly
 *
 * Output form:
 * If a word ladder is found: start->...->dest
 * If not found: no ladder exists
 * 
 * Last edited time:
 * 2018/10/17
 */

#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include<memory>
#include <unordered_map>
#include <unordered_set>
 #include <queue>
#include <vector>
using namespace std;

//a data structure to store the word itself and those differing one letter with it
struct word
{
public:
    string id;//Point
    unordered_set<string>neighbor;//Edge
    string edgeTo;
    word(string s) :id(s) {}
    word() = default;
};

//resolve a string to its standard form
string resolve(string temp);

//build connection between two word if it exists
void build_connection(word& w1, word&w2);

//build connection among a word set
void build_connection(unordered_map<string, word> &english);

//judge if two words differ one letter
bool is_one_differ(string a, string b);

//use BFS to form a word ladder from a built word set
vector<string> build_word_ladder(unordered_map<string, word> &english, string start, string dest);

//output the word ladder
void show_ans(vector<string> &ans);

//input and start function
void input_words(string &start, string &dest);

// read lexicon from file
void read_english(unordered_map<int, unordered_map<string, word>> &english);

vector<string> build_word_ladder(unordered_map<string, word> &english, string start, string dest)
{
    //if in different size there is no ladder
    if (start.size() != dest.size())return vector<string>();
    //transform word string to ptr to word object
    auto st = &(english[start]);
    auto dt = &(english[dest]);
    //mark whether the point need to be visited
    unordered_set<string> marked;
    //store the point that need to be visited
    queue<word*> Q;
    //init with start point
    Q.push(st);
    while (!Q.empty())
    {
        //visit a marked point
        auto out = Q.front();
        Q.pop();
        for (auto& x : out->neighbor)
        {
            //for unvisited point, store the route and mark the point
            if (marked.count(x) == 0)
            {
                marked.insert(x);
                english[x].edgeTo = out->id;
                Q.push(&(english[x]));
            }
        }
        //having found the dest
        if (marked.count(dest) != 0)
            break;
    }
    //no path from start to dest
    if (marked.count(dest) == 0)
        return vector<string>();
    vector<string> res;
    //from des to build the path until start
    for (auto cur = dest; cur != start; cur=english[cur].edgeTo)
    {
        res.push_back(cur);
    }
    res.push_back(start);
    return res;
}

void show_ans(const vector<string> &ans)
{
    if (ans.empty())//the ladder is empty
    {
        cout << "no ladder exists\n\n" ;
        return;
    }
    cout << "Found ladder:";
    cout << ans.back();// the ladder is stored from backward 
    for (auto itr = ans.rbegin()+1; itr != ans.rend(); itr++)
    {
        cout << "->" << *itr;
    }
    cout << endl
         << endl;
}

void input_words(string &start, string &dest)
{
    cout << "Enter start word (enter ':q' to quit):";
    cin >> start;
    start = resolve(start);//transform to standard
    if (start == ":q")
    {
        exit(0);
    }
    cout << "Enter destination word:";
    cin >> dest;
    dest = resolve(dest);
}

string resolve(string temp)
{
    string res;
    for (auto p = temp.begin(); p != temp.end(); p++)
    {
        if (isalpha(*p)) res.push_back(tolower(*p));//ignore all letter if not alpha and make all alpha to lower form
    }
    return res;
}

bool is_one_differ(string a, string b)
{
    if (a.size() != b.size())//no possibilities
        return false;
    bool first = true;//judge if only one difference
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            if (first)
                first = false;
            else
                return false;//with more than one differences
        }
    }
    if (!first)return true;//with no difference
    else return false;
}

void build_connection(word& w1, word&w2)
{
    if (is_one_differ(w1.id, w2.id)&&w1.neighbor.count(w2.id)==0)//if not built
    {
        w1.neighbor.insert(w2.id);
        w2.neighbor.insert(w1.id);
    }
}

void build_connection(unordered_map<string, word> &english)
{
    //build connections for each two words if they exist
    for (auto& w : english)
        for (auto &y : english) 
            build_connection(w.second, y.second);
}

void read_english(unordered_map<int, unordered_map<string, word>> &english)
{
    while (true)
    {
        cout << "Please enter filename containing source text:";
        string file_name;
        cin >> file_name;
        ifstream file_input(file_name.c_str());
        if (file_input.is_open())//check until the file is open well
        {
            string temp_word;
            while (getline(file_input, temp_word))
            {
                auto res = resolve(temp_word);//transform to standard form
                english[res.size()].emplace(res,word(res));//classify them with size
            }
            break;
        }
    }
}

int main()
{
    //mark whether a word set is built
    bool built[1000]{0};
    //a series of word set classified by size
    //key:the word itself ,value: the word object that store some information about the word
    unordered_map<int, unordered_map<string, word>> size_classified_english;
    //read from the text
    read_english(size_classified_english);
    string start, dest;
    //reading until input 'q'
    while (true)
    {
        input_words(start, dest);
        //if in different size or one of the word doesnt exist
        if (start.size() != dest.size()||
            size_classified_english[start.size()].count(start)==0||
            size_classified_english[start.size()].count(dest) == 0
            ) { cout << "no ladder exists\n\n"; continue; }
        //check if a word set is built
        else if (!built[start.size()])
            {
                built[start.size()] = true;
                build_connection(size_classified_english[start.size()]);
            }
        //show the word ladder that caculated by the word set
        show_ans(build_word_ladder(size_classified_english[start.size()], start, dest));
    }
    return 0;
}
