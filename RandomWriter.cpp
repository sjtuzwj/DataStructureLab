/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name:Zhu Wenjie
 * Student ID:517021910799
 * This file is the starter project for the random writer problem of Lab 2.
 * The project use a model called Markov model, which will determine the future state only related to the current state
 * It can be regarded as a state machine, and the current state means the chars before with the size of order which we call them seed
 * The state machine will randomly go to one of all the next states (stored in vector)
*  and output next char which will change the state by updating the state.
 * The project will create a random article based on a text with maxlen of 2000
 * Input:
 * filename(should be a string)
 * order(should be an integer in [0,10]) if you need higher order, it is also legal.
 * Output:
 * An random article whose initial seed is the most fluent char sequence.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <random>
using namespace std;
#define MAX_CHAR_NUM 2000

ofstream fout("output.txt"); 
random_device r;
default_random_engine e(r());
// get filename
void read_file(ifstream &fin);

// get order number
int read_order();

// build model
void model_read(ifstream &fin, const int &order, map<string, vector<char>> &model);

// write
void random_write(const int &order, map<string, vector<char>> &model);

void read_file(ifstream &fin)
{
    cout << "Please input your file\n";
    //get file name
    string filename;
    cin >> filename;
    fin.open(filename);
}

int read_order()
{
    cout << "Please input the order of Markov model\n";
    int order;
    while (!(cin >> order))
    {
        cout << "Try again\n";
        char ch;
        while ((ch = getchar()) != '\n');//Swallow all wrong input
        cin.clear();//reset the fail bit
    }
    return order;
}

void model_read(ifstream &fin, const int &order, map<string, vector<char>> &model)
{
    fin.clear();
    //store all the character before
    string pre;
    char ch;
    while (fin.get(ch)) 
    {
        pre.push_back(ch);
        //to ensure the seed has enough length
        if (pre.size() > order)
        {
            //the key for order 0 is all '\0'
            if (order == 0)model[""].push_back(ch);
            else
            {
                //get seed ending with the last 2 ch with the size of order
                string seed(pre.end() - order - 1, pre.end() - 1);
                //to prevent EOF condition the ch is not initialized
                if (!fin) return;
                //add a possibility to the future state of seed 
                model[seed].push_back(pre.back());
            }
        }
    }
}

void random_write(const int &order, map<string, vector<char>> &model)
{
    string seed;
    unsigned max=0;
    //find the most frequent ch
    for(auto& x: model){
        if (x.second.size() > max) {
            max = x.second.size();
            seed = x.first;
        }
    }
    fout << seed;
    int count = seed.size();
    //the seed exist and the length of article is shorter than maxlen
    while (model.count(seed) != 0 && count <= MAX_CHAR_NUM)
    {
        //get all possible output at current state
        auto &next = model[seed];
        //random sample
        uniform_int_distribution<int>u(0, next.size()-1);
        char nextch = next[u(e)];
        //get the next seed
        if (order == 0)seed = "";
        else seed = string(seed.begin()+1,seed.end()) + nextch;
        fout << nextch;
        //update the count of char
        count++;
    }
    
}

int main()
{
    ifstream fin;
    read_file(fin);
    while (!fin.is_open())
    {
        read_file(fin);
    }
    while (true)
    {
        int order = read_order();
        map<string, vector<char>> model;
        model_read(fin, order, model);
        random_write(order, model);
    }
    return 0;
}
