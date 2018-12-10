// Clare DuVal
// CPSC 2120 001
// Lab 10: Text Compression using a Trie

#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

// A node in our encoding trie
struct Node {
  char letter; // The character labeling the edge to this node's parent
  int first_child; // -1 if none
  int next_sibling; // -1 if none
  Node (char _letter, int _next_sibling) {
    letter = _letter; next_sibling = _next_sibling; first_child = -1;
  }
};

int main(void)
{
  vector<Node> nodes;
  nodes.push_back(Node(0,-1));
  int curnode = 0, c;

  while (c != EOF) {
    // Read one character from standard input, whitespace included
    c = cin.get();

    // Walk down the tree, adding a child if we get stuck
    int n = nodes[curnode].first_child;
    while (n != -1 && nodes[n].letter != c) n = nodes[n].next_sibling;

    // If we found a child matching c, step to it
    if (n != -1) curnode = n;
    else {

      // Add c as a new child and print out its information. Reset to root
      nodes.push_back(Node(c, nodes[curnode].first_child));
      nodes[curnode].first_child = nodes.size()-1;
      cout << curnode << " " << c << "\n";
      curnode = 0;
    }
  }

  return 0;
}
