//Clare DuVal
//CPSC 2120 001
//HW 4: Nearest-Neighbor Classification using KD-Trees (11-Dimensions)

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

//Holds the wines index and the data of the qualities, NOTE: data[0] = label
struct Point {
  double label;
  double *data;
};

//Nodes that make up the kd tree of wine Points
struct Node {
  Point * key;
  Node * left;
  Node * right;
  Node(Point * k) { key = k; left = right = NULL; }
};

int N, D, K; // N # of wines, D # of data qualities, K # of nearest neighbors
double ALPHA = 0.1; // Used in weighted average formula

// Holds the wines
Point *P;

//Randomized vector of N number to utalize random insert
vector <int> numbers;

// Priority queue of the nearest neighbors with smallest dist to a Point
// Prioritize to only hold Q.size = K nearest neighbors
priority_queue<pair<double, Point *>> Q;

//Inserts a Point into a kd tree
Node * insert(Node * kd, Point * key, int dim) {
  if (kd == NULL) return new Node(key);
  if(key->data[dim] < kd->key->data[dim])
    kd->left = insert(kd->left, key, (dim+1)%D);
  else
    kd->right = insert(kd->right, key, (dim+1)%D);
  return kd;
}

//Search the KD tree, while inserting the K nearest neighbors into Q
void search_KD(Node * kd, int idx, unsigned dim) {

  // Move on if the tree is NULL
  if (!kd) return;

  // Calculates the distance from current to comparing node
  double dist = 0.0;
  for (int d = 0; d < D; d++) {
    dist +=  pow(P[idx].data[d] - kd->key->data[d], 2);
  }
  dist = sqrt(dist);

  // Ignore if the comparing node is itself,
  // else push the weighted distance with the neightbor into the PQ
  if (kd->key != &P[idx]) Q.push(make_pair(dist, kd->key));

  // ONLY keep K nearest neighbors
  if (Q.size() > K) Q.pop();

  //Search the remaining tree by pruning
  double dist_d = abs(P[idx].data[dim] - kd->key->data[dim]);
  if (P[idx].data[dim] > kd->key->data[dim]) {

    search_KD(kd->right, idx, (dim+1)%D);
    if (dist_d < Q.top().first) { search_KD(kd->left, idx, (dim+1)%D); }

  } else {

    search_KD(kd->left, idx, (dim+1)%D);
    if (dist_d < Q.top().first) { search_KD(kd->right, idx, (dim+1)%D); }
  }
}

// MAIN
int main(int argc, char *argv[]) {

  if (argc != 3) {
    cout << "Usage: wine <data file> <k>\n";
    return 0;
  }

  // Read input data
  ifstream fin(argv[1]);
  K = atoi(argv[2]);
  fin >> N >> D;
  P = new Point[N];
  for (int i=0; i<N; i++) {
    fin >> P[i].label;
    P[i].data = new double[D];
    for (int j=0; j<D; j++)
      fin >> P[i].data[j];
  }
  fin.close();

  // Normalize data in each coordinate to mean 0 and std dev 1
  for (int j=0; j<D; j++) {
    double mean = 0.0;
    for (int i=0; i<N; i++) mean += P[i].data[j];
    mean /= N;
    for (int i=0; i<N; i++) P[i].data[j] -= mean;
    double var = 0.0;
    for (int i=0; i<N; i++) var += pow(P[i].data[j],2);
    double stddev = sqrt(var/N);
    for (int i=0; i<N; i++) P[i].data[j] /= stddev;
  }

  //Randomize the numbers that are inserted into the kd tree
  for (int i = 0; i != N; i++) {
    numbers.push_back(i);
  }
  random_shuffle(numbers.begin(), numbers.end());

  //Insert each point into the kd tree
  Node * kd = NULL;
  for (int i : numbers) {
    kd = insert(kd, &P[i], 0);
  }

  //Create the priority queue for each point
  double num, denom, err = 0.0;
  for (int i : numbers) {
    num = 0.0;
    denom = 0.0;
    search_KD(kd, i, 0);

    //Use the K neighbor's data to calculate the weighted/guessed average
    while (!Q.empty()) {
      //Numerator = Sumation(e^(-dist*alpha) * closest average label)
      num += exp(-(Q.top().first*ALPHA)) * Q.top().second->label;
      //Denominator = Sumation(e^(-dist*alpha))
      denom += exp(-(Q.top().first)*ALPHA);
      Q.pop();
    }

    //Standard error is (Actual - weighted)^2
    err += pow(P[i].label - (num/denom), 2);
  }

  //Print average standard error
  cout << (err / N) << endl;

  return 0;
}
