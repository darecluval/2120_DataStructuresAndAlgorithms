//Clare DuVal
//CPSC 2120 001
//Homework 3: Candy Optimization

#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

struct Bag {
  int weight = 0;
  int tastiness = 0;
};

struct Candy {
  int weight;
  int tastiness;
  int bag = 0;
};

//Global Variables
int const NUM_BAGS = 3;
int const MAX_WEIGHT = 2000;
int const NUM_PIECES = 16;
Bag Bags [NUM_BAGS+1];
Candy Candies [NUM_PIECES];

//FUNCTIONS FOR FIRST APPROACH
//Return the combined total of each bag's ending weight
int firstApproach() {
  int totTasty = 0;

  //Loop through candies to add top pieces to bags //SWAP
  for (int i = 0; i < NUM_PIECES; i++) {
    for (int j = 1; j < (NUM_BAGS+1); j++) {
      if ((Candies[i].bag == 0) && (Bags[j].weight +  Candies[i].weight <= MAX_WEIGHT)) {
        Candies[i].bag = j;
        Bags[j].weight += Candies[i].weight;
        Bags[j].tastiness += Candies[i].tastiness;
        totTasty += Candies[i].tastiness;
      }
    }
  }

  //Return the n-th bag's tastiness value
  return totTasty;
}

//Resets the Candies's bag number
void resetCandies() {
  for (int i = 0; i < NUM_PIECES; i++) {
    Candies[i].bag = 0;
  }
  for (int j = 0; j < 4; j++) {
    Bags[j].weight = 0;
    Bags[j].tastiness = 0;
  }
}

//FUNCTIONS FOR APPROACH TWO
int refine(int currTasty) {

  //100 Swaps
  for (int i = 0; i < 100; i++) {

    //Swap Neighbors, or ends
    for (int j = 0; j < NUM_PIECES; j++) {
      swap(Candies[j], Candies[(j+1) %NUM_PIECES]);

      //If the new Tasty Level is better swaped, update bestTasty
      //resetCandies();
      int newTasty = firstApproach();
      if (newTasty > currTasty) {
        currTasty = newTasty;

      } else { //If not, swap them back
      swap(Candies[j], Candies[(j+1) %NUM_PIECES]);
      }
    }
  }

  //Return the refined tastiness
  return currTasty;
}

//Iterative Refinement
int secondApproach() {

  int randTasty, newTasty, bestTasty = 0;

  //100 Iterations
  for (int j = 0; j < 1000; j++) {

    //Shuffle: For each candy, swap with random
    for (int i = 0; i < NUM_PIECES; i++) {
      swap(Candies[i], Candies[rand() %NUM_PIECES]);
    }

    //Find the tasty of the shuffle
    randTasty = firstApproach();

    //Return the tasy of the refinement, set that to best if it's better
    newTasty = refine(randTasty);
    if (newTasty > bestTasty) { bestTasty = newTasty; }

    resetCandies();

  }

  //Return the best tastiness
  return bestTasty;
}

//val(sofar) + val(leftover) < best
//Pruned Exhaustive Search
int thirdApproach(int c) {

  int retTasty, totTasty = 0;
  bool sym;

  //Base case returns the total tastiness of the abgs
  if (c == NUM_PIECES) {
    return Bags[0].tastiness + Bags[1].tastiness + Bags[2].tastiness;
  }

  //Consider each bag, and none
  for (int j = 0; j < NUM_BAGS+1; j++) {

    //If two bags have the same weight, only check one
    sym = false;
    for (int b = 0; b < j; b++) {
      if (Bags[j].weight == Bags[b].weight) {
        sym = true;
      }
    }

    if (!sym) {

      //If at the end, don't use the candy
      if (j == NUM_BAGS) {
        totTasty = max(thirdApproach(c+1), totTasty);

      } else if (Bags[j].weight +  Candies[c].weight <= MAX_WEIGHT) {
        //Temp add the candy
        Bags[j].weight += Candies[c].weight;
        Bags[j].tastiness += Candies[c].tastiness;

        //Recurse on next candy
        totTasty = max(thirdApproach(c+1), totTasty);

        //Take out the candy
        Bags[j].weight -= Candies[c].weight;
        Bags[j].tastiness -= Candies[c].tastiness;

      }
      }
    }
  return totTasty;
}

bool operator<(const Candy &A, const Candy &B) {
  double ratioA = (double) A.tastiness/A.weight;
  double ratioB = (double) B.tastiness/B.weight;
  return (ratioA > ratioB);
}

//MAIN
int main(void) {

  ifstream candyF("candy.txt");

  //int leftOver = 0;

  //Read candies' weight and num into table
  //Add the weight, tastiness, and taken (off/on) to the i-th candy
  for (int i = 0; i < NUM_PIECES; i++) {
    candyF >> Candies[i].weight;
    candyF >> Candies[i].tastiness;
    //leftOver += Candies[i].tastiness;
  }

  //Print results of each approach
  sort(Candies, Candies+NUM_PIECES);
  cout << "Greedy: " << firstApproach() << endl;
  resetCandies();
  cout << "Refinement: " << secondApproach() << endl;
  resetCandies();
  sort(Candies, Candies+NUM_PIECES);
  cout << "Exhaustive: " << thirdApproach(0) << endl;

  candyF.close();
  return 0;
}
