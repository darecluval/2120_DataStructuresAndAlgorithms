//Clare DuVal
//Lab 06 - Goal One
//CPSC 2121 006

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

//Struct for vector and bst
struct Node {
  int height;
  int pos;
  int rank;
  int leftD;
  int rightD;
};

struct Leaf {
  int height;
  int pos;
  Leaf * left;
  Leaf * right;
  Leaf(int h, int p) {height = h; pos = p; left = right = NULL;}
};

//Sort from height high to low
bool sorter(const Node &lhs, const Node &rhs) {
  return (lhs.height > rhs.height);
}

//Sort from rank low to high
bool sorterR(const Node &lhs, const Node &rhs) {
  return (lhs.rank < rhs.rank);
}


//Split
void split(Leaf * T, int pos, Leaf **L, Leaf **R) {

  //NULL cases
  if (T == NULL) {
    *L = NULL;
    *R = NULL;
    return;
  }

  //Split left tree if r < T->size
  if (pos < T->pos) {
    split(T->left, pos, L, &T->left);
    *R = T;
  }

   else {
    //Split right tree if r >= T->size
    split(T->right, pos, &T->right, R);
    *L = T;
  }
}

//Insert
Leaf * insert(Leaf * T, int height, int pos) {

  // otherwise, insert_keep_balanced k recursively left or right of the root of T
  if (T == NULL) return new Leaf(height, pos);

  if (rand() % (T->pos + 1) == 0) {
    // insert at root
    Leaf *new_root = new Leaf(height, pos);
    split(T, pos, &new_root->left, &new_root->right);
    return new_root;
  }

  // otherwise insert recursively...
  if (pos < T->pos) { T->left = insert(T->left, height, pos); }
  else { T->right = insert(T->right, height, pos); }
  return T;
}

//PRED
Leaf * pred(Leaf * T, int pos) {
  if (T == NULL) return NULL;
  if (T->pos == pos) return T;

  if (pos < T->pos) return pred(T->left, pos);
  else {
    Leaf * temp = pred(T->right, pos);
    if (temp == NULL) return T;
    else return temp;
  }
  return NULL;
}

//SUCC
Leaf * succ(Leaf * T, int pos) {
  if (T == NULL) return NULL;
  if (T->pos == pos) return T;

  if (pos > T->pos) return succ(T->right, pos);
  else {
    Leaf * temp = succ(T->left, pos);
    if (temp == NULL) return T;
    else return temp;
  }
}



//Main
int main(void) {

  int height, pos, currH, count = 0;
  Leaf * curr = NULL;

  //Read in the file
  ifstream text("sample.in");

  //Create a vector of count amount of Nodes
  text >> count;

  //Generate random number of count

  cout << "Please enter a size n to generate random numebrs: ";
  cin >> count;
  cout << endl;

  vector<Node> N(count);
  Leaf * T = NULL;
  for (unsigned i = 0; i < count; i++) {

    //Read in position and height
    pos = rand() % (10*count);
    height = rand() % (10*count);
    //text >> pos >> height;

    //Insert into vector (to be sorted)
    N[i].pos = pos;
    N[i].height = height;
    N[i].rank = (i + 1);

  }

  //Sort the array
  sort(N.begin(), N.end(), sorter);

  //Print the sorted vector
  for (Node &n : N) {
    cout << n.height << endl;
  }

  Leaf * pre = NULL, * suc = NULL;

  //Print the longest distance to the left and to the right for each position
  for (int i = 0; i < count; i++) {

    pre = pred(T, N[i].pos);
    suc = succ(T, N[i].pos);

    if (pre != NULL) N[i].leftD = N[i].pos - pre->pos;
    else N[i].leftD = -1;

    if (suc != NULL) N[i].rightD = suc->pos - N[i].pos;
    else N[i].rightD = -1;

    T = insert(T, N[i].height, N[i].pos);
  }

  sort(N.begin(), N.end(), sorterR);

  for (int i = 0; i < count; i++) {

    //Pring pred
    cout << "The line of sight from positon " << i+1 << " to the left is: " << N[i].leftD;

    cout << endl;

    //Print succ
    cout << "The line of sight from position " << i+1 << " to the right is: " << N[i].rightD;

    cout << endl << endl;

  }

  text.close();

  return 0;
}
