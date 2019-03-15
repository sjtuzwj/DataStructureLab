#include<string>
#include<memory>
using namespace std;

/*TST
In the left , store TST whose root less than this
In the mid ,store next TST
In the right, store TST whose root larger than this
The depth is only computed with MID subtree.
Deal with N strings( average length: w) with R-sized alphabet 
Time Complexity: 1.39lgN  -> O(lgN)
Space Complexity: 64 - 64Nw -> O(N)
Compare to RST, it plays well when R is large.
*/
class Trie {
    struct Node;
    using Np = shared_ptr<Node>;
    Np root;
    struct Node {
        char c='\0';
        bool hasitem=false;
        Np left, right, mid;
        Node() = default;
        Node(char ch) :c(ch) {};
    };
public:
    bool search(string word) {
        return search(root, word, 0);
    }
    bool search(Np x, string key, size_t d) {
        if (x == nullptr) return false;
        char c = key[d];
        //special case
        if (c == '*') {
            if (d < key.length() - 1)
                return search(x->mid, key, d + 1) || search(x->left, key, d) || search(x->right, key, d);
            else 
                return x->hasitem;
        }
        if (c < x->c)	return search(x->left, key, d);
        else if (c > x->c)	return search(x->right, key, d);
        else if (d < key.length() - 1)	return search(x->mid, key, d + 1);
        else return x->hasitem;
    }

    void insert(string word) {
        root = insert(root, word, 0);
    }

    Np insert(Np x, string key, size_t d) {
        char c = key[d];
        if (x == nullptr)  x = make_shared<Node>(c);
        if(c < x->c) x->left = insert(x->left, key, d);
        else if (c > x->c) x->right = insert(x->right, key, d);
        else if (d < key.length() - 1)	x->mid = insert(x->mid, key, d + 1);
        else x->hasitem = true;
        return x;
    }
};
