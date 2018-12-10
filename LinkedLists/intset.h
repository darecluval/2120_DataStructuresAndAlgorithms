#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

struct Node {
  int Number;
  Node *next;
  Node (int num, Node *A) {Number = num; next = A;}
};

class Intset {

 private: 
  Node* head;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
