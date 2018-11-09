//Clare DuVal
//CPSC 2020 001
//Lab 9: Wolf, Goat, and Cabbage River Solution

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <string>

using namespace std;

typedef int state;
string stateOf[4] = {"wolf", "goat", "cabbage", "you"};

// GENERIC
map<state, bool> visited;                   // have we queued up this state for visitation?
map<state, state> pred;                     // predecessor state we came from
map<state, int> dist;                       // distance (# of hops) from source node
map<state, vector<state>> nbrs;             // vector of neighboring states
map<pair<state,state>,string> edge_label;   //edge labels from each pair of states

// GENERIC (breadth-first search, outward from curnode)
void search(state source_node) {
  queue<state> to_visit;
  to_visit.push(source_node);
  visited[source_node] = true;
  dist[source_node] = 0;

  while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode]) {
      if (!visited[n]) {
        pred[n] = curnode;
	      dist[n] = 1 + dist[curnode];
	      visited[n] = true;
	      to_visit.push(n);
      }
    }
  }
}

//Print the change of states
void print_state(state s)
{
  cout << edge_label[make_pair(pred[s], s)] << endl;
}

//Print the viable path
void print_path(state s, state t) {
  if (s != t) print_path(s, pred[t]);
  print_state(t);
}

//Makes sure the change of states is viable
bool meets_conditions(state s, state t) {
  //Wolf and goat unsupervised
  if ((((s>>0)&1) == ((s>>1)&1)) && (((s>>3)&1) != ((s>>0)&1))) return false;
  if ((((t>>0)&1) == ((t>>1)&1)) && (((t>>3)&1) != ((t>>0)&1))) return false;

  //Goat and cabbage unsupervised
  if ((((s>>1)&1) == ((s>>2)&1)) && (((s>>3)&1) != ((s>>1)&1))) return false;
  if ((((t>>1)&1) == ((t>>2)&1)) && (((t>>3)&1) != ((t>>1)&1))) return false;

  else return true;
}

//Checks is two states is a viable change
bool are_neighbors(state a, state b) {

  string label = "";
  int count = 0;

  //I need to cross the river each time
  if (((a >> 3)&1) != ((b >> 3)&1)) {
    label += "I cross the river" ;
    count++;
  }
  else { return false; }

  //Check if the wolf, goat, or cabbage cross
  for (int i=0; i<3; i++) {

    //If an item crosses the river, it must cross the same direction as me
    if ((((a >> i)&1) != ((b >> i)&1))) {
          if (((b>>i)&1) == ((b >> 3)&1)) {
            label += " with  the " + stateOf[i];
            count++;}
          else return false;
    }
  }

  if (!meets_conditions(a, b)) return false;

  //Only max of two characters may cross the river at once
  if (count <= 2) {

    //Set edge label of the change of states
    label += ".\n";
    edge_label[make_pair(a, b)] = label;
    return true;

  } else { return false; }

}

//The nbrs data structure is all that is used
// from this point onward to specify the graph.
void build_graph(void) {
  vector<state> states;
  state s, t;

  //0-15 represent binanry states of each character
  for (s = 0; s < 16; s++) {
    states.push_back(s);
  }

  //Check if two states are neighbors
  for (int s : states)
    for (int t : states)
      if (are_neighbors(s,t)) {
	       nbrs[s].push_back(t);
      }
}

//MAIN
int main(void) {

  build_graph();
  state start = 0, end = 15;
  search (start);

  cout << "Initially, the wolf, goat, cabbage, and I are on the left side of the river." << endl;
  print_path (start, end);
  cout << "Now, the wolf, goat, cabbage, and I are on the right side of the river." << endl;

  return 0;
}
