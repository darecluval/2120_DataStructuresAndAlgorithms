// Clare DuVal
// CPSC 2120 001
// Lab 10: Text Compression using a Trie

#include <iostream>
#include <cstdio>
#include <string>
#include <map>
using namespace std;

// A node in our decoding trie
struct Node {
  int letter; // The character labeling the edge to this node's parent
  Node *parent;
  Node (int _letter, Node * _parent) {
    letter = _letter; parent = _parent;
  }
};

//Map to track the placement of each Node
map <int, Node *> order;

int main(void) {

  Node *root = new Node(0, NULL);
  order[0] = root;

  int let, par, num = 1;

  //Read in the parent ID and letter
  while (cin >> par >> let) {

    //Make a new node with the par and letter in the map
    order[num++] = new Node(let, order[par]);

    string print = "";

    //Add to the string all the parents (to the root) that preceed the current letter
    for (Node * d = order[num-1]; d != NULL; d = d->parent) {
        print = string(1, (char)d->letter) + print;
    }

    //Print the result of the trie
    cout << print;
  }

  return 0;
}
