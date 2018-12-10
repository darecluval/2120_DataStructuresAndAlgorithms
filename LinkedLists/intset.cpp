#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{
  head = NULL;
}

Intset::~Intset(){
}

/* Return true if key is in the set */
bool Intset::find(int key)
{
  //Linked List
  Node *A =  head;
  while (A != NULL) {
    if (A->Number == key) return true;
    A = A->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
  //ARRAY
  assert (!find(key));

  /* Shift up elements in array to make room for new element... */
  Node* A = head;
  Node* B = head;

  //First number and when key is smaller than all numbers
  if (A == NULL || A->Number >= key) {

    Node* temp1 = new Node(key, A);
    head = temp1;

  }

  //All other numbers
  else {

    while (B->next != NULL && (B->next->Number < key)) {
      B = B->next;
    }

    Node* temp1 = new Node(key, B->next);
    B->next = temp1;

  }

}

/*Removes a key from the set. Returns an error if the key is not in the set*/
void Intset::remove(int key)
{
  assert (find(key));

  //Linked List
  Node* A = head;
  Node* B = head;

  A = A->next;

  if (!A->next) {
    delete A;
  } else {

    while (A->Number != key) {
      A = A->next;
      B = B->next;
    }

    A = A->next;

    Node* rem = B->next;
    B->next = A;

    delete rem;
  }
}

//Prints the set in order
void Intset::print(void)
{
  Node* A = head;

  //Linked List
  while (A != NULL) {
    cout << A->Number << "\n";
    A = A->next;
  }
}
