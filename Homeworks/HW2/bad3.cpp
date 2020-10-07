#include <iostream>
#include <cstdlib>
using namespace std;

struct Node {
  int val;
  int size;
  Node * left;
  Node * right;
  Node (int v) { val = v; size = 1; left = right = NULL; }
};

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// Split tree T on rank r into tree L (containing ranks < r) and 
// a tree R (containing ranks >= r)
void split(Node *T, int r, Node **L, Node **R)
{

  int rank;
  //NULL cases
  if (T == NULL) {
    *L = NULL;
    *R = NULL;
    return;
  }
  if (T->left == NULL) rank = 0;
  else rank = T->left->size;

  //Split left tree if r < T->size
  if (r <= rank) {
    split(T->left, r, L, &T->left);
    *R = T;
  }

   else {
    //Split right tree if r >= T->size
    split(T->right, r - rank - 1, &T->right, R);
    *L = T;
  }
  fix_size(T);
}

// insert value v at rank r
Node * insert(Node *T, int v, int r)
{

  int rank;
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_keep_balanced k recursively left or right of the root of T
  if (T == NULL) return new Node(v);
  if (T->left == NULL) rank = 0;
  else rank = T->left->size;

  if (rand() % (T->size + 1) == 0) {
    // insert at root
    Node *new_root = new Node(v);
    split(T, r, &new_root->left, &new_root->right);
    fix_size(new_root);
    return new_root;
  }

  // otherwise insert recursively...
  if (r <= rank) { T->left = insert(T->left, v, r); }
  else { T->right = insert(T->right, v, r - rank - 1); }
  fix_size(T);
  return T;
}


// prints out the inorder traversal of T (i.e., the contents of T in sorted order)
void print_inorder(Node *T)
{
  if (T == NULL) return;
  print_inorder(T->left);
  cout << T->val << "\n";
  print_inorder(T->right);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "Usage: bad1 <input size>\n";
    return 0;
  }

  int N = atoi(argv[1]);  // get first command-line argument
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }

  // Generate and print bad input of size N for prog1
  // (currently just generates an input of N random numbers)
  cout << N << "\n";

  int tableSize = 10000;

  Node * T = NULL;

  int size;
  for (int k = 1; k < (N+1); k++) {
    int size = 123456789 % k;
    T = insert(T, k, size);
  }

  print_inorder(T);


  return 0;
}
