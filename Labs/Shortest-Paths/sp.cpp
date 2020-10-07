  //Clare DuVal
//CPSC 2120 001
//Lab 11: Single-Source Shortest Paths: Dijkstra’s Algorithm

//Note: McAdams is Clemson's School of Computing. We are trying
// to find the path from the neighbors with the least cost

#include <cstdio>
#include <cstdlib>
#include <queue>
#include <map>
#include <algorithm>
#include <vector>
#include <limits>
using namespace std;

int i, N, M; // Number of nodes and edges

map <int, vector<int>> neighbors;         // Outgoing neighbors of each node
map <pair<int,int>, double> travel_time;  // Travel times of edges
map <int, double> cost;                   // Node's cost distance
map <int, int> visited;                   //

int MCADAMS = 17731931;

//Assumes usa_network.txt.gz has been placed on
// the /tmp folder of the local computer
void read_input(void) {
  FILE *fp = popen ("zcat /tmp/usa_network.txt.gz", "r");
  fscanf (fp, "%d %d", &N, &M);
  printf ("Reading input");

  for (int i = 0; i < M; i++) {
    if (i % 1000000 == 0) { printf ("."); fflush(stdout); }

    int source, dest;
    double t;
    fscanf (fp, "%d %d %lf", &source, &dest, &t);
    neighbors[source].push_back(dest);
    travel_time[make_pair(source, dest)] = t;
  }

  fclose(fp);
  printf (" Done.\n" ); fflush(stdout);

}

//Short pair of double and ints
typedef pair<double, int> pdi;

int main(void) {

  read_input();

  //Priority Queue
  priority_queue<pdi, vector<pdi>, greater<pdi>> Q;

  //Create the cost for each, MCADAMS is exception of zero
  for (int i = 0; i < N; i++) {
    if (i == MCADAMS) { cost[i] = 0; }
    else {cost[i] = 2147483647.0;} // MAX DOUBLE
    visited[i] = 1;
  }

  //Begin at McAdams
  int j, i = MCADAMS;
  Q.push(make_pair(0, i));

  //Dijkstra's Algorithm
  while(!Q.empty()) {

    //Chose the next
    i = Q.top().second;
    Q.pop();

    //Find yet-unvisited node i having min label cost[i]
    if (visited[i] == 1) {

      //Set to visited
      visited[i] = 0;

      //For all edges (i, j) outgoing from i:
      for (int j:neighbors[i]) {
        if ((cost[i] + travel_time[make_pair(i,j)]) < cost[j]) {
          //Current cost += cost of most beneficial neighbor
          cost[j] = cost[i] + travel_time[make_pair(i,j)];
          Q.push(make_pair(cost[j], j));
        }
      }
    }
  }

  //Print answer in terms of hours
  printf("%lf", cost[i] / 3600.0);
  return 0;
}
