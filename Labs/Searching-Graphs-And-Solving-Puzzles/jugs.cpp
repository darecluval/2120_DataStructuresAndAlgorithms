//Clare DuVal
//CPSC 2020 001
//Lab 9: 3 and 4 Gallon Jugs, Make 5 Gallons

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <queue>
using namespace std;

// Represents the state of the pair of jugs A and B
typedef pair<int, int> state;

// GENERIC
map<state, bool> visited;                  // have we queued up this state for visitation?
map<state, state> pred;                    // predecessor state we came from
map<state, int> dist;                      // distance (# of hops) from source node
map<state, vector<state>> nbrs;            // vector of neighboring states
map<pair<state,state>,string> edge_label;  //edge labels from each pair of states

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

//Print in the format of "Label. [Amount in A, Amount in B]"
void print_state(state s)
{
  //Base case to print label for all states other than the inital
  if (!(s.first == 0) || !(s.second == 0)) {
    cout << edge_label[make_pair(pred[s], s)];
  }

  //[Amount in A, Amount in B]
  cout << "[" << s.first << ", " << s.second << "]" << endl;
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) print_path(s, pred[t]);
  print_state(t);
}

//Create the edge of pair states and labels
void edge(int a, int b, int c, int d, string label) {
  state jugs = make_pair(a, b);
  nbrs[jugs].push_back(make_pair(c, d));
  edge_label[make_pair(jugs, make_pair(c, d))] = label;
}

//The nbrs data structure is all that is used
// from this point onward to specify the graph
void build_graph(void)
{
  //Go through all viable options
  for (int a = 0; a <= 3; a++) {
    for (int b = 0; b <= 4; b++) {

      //Fill A
      edge(a, b, 3, b, "Fill A.   ");
      //Fill B
      edge(a, b, a, 4, "Fill B.   ");

      //Empty A
      edge(a, b, 0, b, "Empty A.    ");
      //Empty B
      edge(a, b, a, 0, "Empty B.    ");

      //Put A -> B, don't overfill
      int w = min(a, 4-b);
      edge(a, b, a-w, b+w, "Empty A into B.   ");
      //Put B -> A, don't overfill
      w = min(3-a, b);
      edge(a, b, a+w, b-w, "Empty B into A.   ");
    }
  }
}

//MAIN
int main(void)
{

  build_graph();

  state start = make_pair(0,0), end;
  search (start);

  //Run through all possible end senarios until a path is found
  for (int a = 0; a < 4; a++) {
    end = make_pair(a, 5-a);

    //Print the viable path
    if (visited[end]) {
      cout << "Inital: ";
      print_path (start, end);
      break;
    }
  }

  return 0;
}
