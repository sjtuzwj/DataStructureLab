/*
*Thanks for Pro. Robert Sedgewick -Univ. Princeton
*bit IO stream migrated from Algs4
*link: https://github.com/sjtuzwj/Bit-IO-in-Cpp
*/

#include<vector>
#include <iostream>
#include <string>
#include<array>
#include <fstream>
#include<string>
#include<memory>
#include<algorithm>
//#include <cstdlib>
#include<queue>
#include"BinaryStdIn.h"
#include"BinaryStdOut.h"
using namespace std;

BinaryStdOut ofs;
BinaryStdIn ifs;

class Huffman
{
    class Node;
    using Np = shared_ptr<Node>;
    static const int R = 256;//the size of alphabet
private:
    class Node {
    public:
        byte ch;//character
        int freq;//frequency
        Np left, right;
        Node(byte c, int f, Np l, Np r) :ch(c), freq(f), left(l), right(r) {}
        bool isLeaf() { return left == nullptr && right == nullptr; }
        bool compareTo(const Node& that) { return freq > that.freq; }//frequency least, pick first.
    };
public:
    static void expand()
    {
        Np root = readTrie();//read the Huffman tree from stream
        size_t n=ifs.readInt();//read the byte num
        for (size_t i = 0; i < n; i++)
        {
            Np x = root;
            while (!x->isLeaf())//decoding to node
                if (ifs.readBoolean())x = x->right;
                else x = x->left;
            ofs.write(x->ch);//node to char
        }
    }
    static void buildCode(array<string, R>& st, Np x, string s)
    {
        if (x->isLeaf())
        {
            st[x->ch] = s; return;//encoding char to binary mode
        }
        buildCode(st, x->left, s + '0');
        buildCode(st, x->right, s + '1');
    }

    static array<string, R> buildCode(Np root)//build string table(compiler table)
    {
        array<string, R> st{};
        buildCode(st, root, "");
        return st;
    }

    static Np buildTrie(array<int, R>& freq)
    {
        auto cmp = [](Np& lhs, Np& rhs) {return lhs->compareTo(*rhs); };
        priority_queue < Np, vector<Np>, decltype(cmp) > pq(cmp);
        for (int c = 0; c < R; c++)
            if (freq[c] > 0)  pq.push(make_shared<Node>(c, freq[c], nullptr, nullptr));//construct nodes
        while (pq.size()>1)//pick the least 2 child tree and merge.
        {
            Np x = pq.top();
            pq.pop();
            Np y = pq.top();
            pq.pop();
            Np prt = make_shared<Node>('\0', x->freq + y->freq, x, y);
            pq.push(prt);
        }
        return pq.top();
    }

    static void open(string inputFilename, string outputFilename) {
        ofs.open(outputFilename);
        ifs.open(inputFilename);
    }
    static void compress(string inputFilename, string outputFilename) {
        open(inputFilename, outputFilename);
        string tmp = ifs.readString();//all char in the file
        vector<byte> input;//array form
        for (size_t i = 0; i != tmp.size(); i++)
            input.push_back(tmp[i]);
        array<int, R>freq{};
        for (auto& ch: input)//count the frequency
            freq[ch]++;
        Np root = buildTrie(freq);
        auto st=buildCode(root);
        writeTrie(root);//write the Huffman to the output stream
        ofs.write(int(input.size()));//write the byte num
        for (auto& ch : input)//encoding char to Huffman code
        {
            string code = st[ch];
            for (size_t j = 0; j < code.size(); j++)
                if (code[j] == '1')
                    ofs.write(true);
                else ofs.write(false);
        }
        ifs.close();
        ofs.close();
    };
    static Np readTrie()
    {
        if (ifs.readBoolean()) {//leave
            return make_shared<Node>(ifs.readChar(), 0, nullptr, nullptr);
        }
        else {//DLR construct the Huffman tree
            auto l = readTrie();
            auto r = readTrie();
            return make_shared<Node>('\0', 0, l, r);
        }
    }
    static void writeTrie(Np x)
    {
        if (x->isLeaf())
        {
            ofs.write(true);//show it is leaf
            ofs.write(x->ch);
            return;
        }//DLR encode the Huffman tree
        ofs.write(false);//show it is stem
        writeTrie(x->left);
        writeTrie(x->right);
    }
    static void decompress(string inputFilename, string outputFilename) {
        open(inputFilename, outputFilename);
        expand();
        ifs.close();
        ofs.close();
    }
};


void usage(string prog) {
  cerr << "Usage: " << endl
      << "    " << prog << "[-d] input_file output_file" << endl;
  exit(2);
}

int main(int argc, char* argv[]) {
    try {
        int i;
        string inputFilename, outputFilename;
        bool isDecompress = false;
        for (i = 1; i < argc; i++) {
            if (argv[i] == string("-d")) isDecompress = true;
            else if (inputFilename == "") inputFilename = argv[i];
            else if (outputFilename == "") outputFilename = argv[i];
            else usage(argv[0]);
        }
        if (outputFilename == "") usage(argv[0]);
        if (isDecompress) Huffman::decompress(inputFilename, outputFilename);
        else Huffman::compress(inputFilename, outputFilename);
    }
    catch (exception&e)
    {
        cout << e.what()<<endl;
    }
  return 0;
}
