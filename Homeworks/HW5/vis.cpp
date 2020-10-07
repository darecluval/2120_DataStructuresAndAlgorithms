//Clare DuVal
//CPSC 2120 001
//Homework 5: Geometric Shortest Paths

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <map>
#include <cmath>
#include <queue>
#include "geom.h"
using namespace std;

double ERROR = 0.001;
double PI = 3.141592653589793238462643383279;

Vec2D mcadams = Vec2D(1540, 408);
Vec2D my_pos = Vec2D(895, 519);
Vec2D cory = Vec2D(1295, 200);
double my_angle = 0.0;
int num_vertices;

vector<Segment2D> segs;
vector<Segment2D> coryLOS;
vector<Segment2D> path;
vector<int> buildings;
vector<pair<double,double> > all;

bool operator<(Vec2D a, Vec2D b) {
  if (a.x == b.x) {
    return a.y < b.y;
  } else return a.x > b.x;
}

bool operator<(Segment2D a, Segment2D b) {
  if (a.p.x == b.p.x && a.p.y == b.p.y) {
    return a.q < b.q;
  } else return a.p < b.p;
}

map<Vec2D, vector<Vec2D>> nbrs;
map<Segment2D, double> cost;
map<Vec2D, int> visited;

priority_queue<double, vector<Vec2D>> Q;

// Code to visualize output...
void xfig_header(void)
{
  printf ("#FIG 3.2  Produced by xfig version 3.2.5\nLandscape\nCenter\nInches\nLetter\n100.00\nSingle\n-2\n1200 2\n");
}

// Objects with larger depth are drawn first, so they are occluded by objects with smaller depth
void line(int x1, int y1, int x2, int y2, int depth, int thickness)
{
  int color = 0; // small integers are different colors

  printf ("2 1 %d %d %d 7 %d -1 -1 0.000 0 0 -1 0 0 2\n", 0, thickness, color, depth);
  printf ("\t%d %d %d %d\n", x1, y1, x2, y2);
}

// fill_percent_white = 100 for a white-filled circle, 0 for black-filled circle
void circle(int x, int y, int radius, int depth, int fill_percent_white)
{
  printf ("1 3 0 1 0 7 %d -1 %d 0.000 1 0.0000 %d %d %d %d %d %d %d %d\n",
	  depth, fill_percent_white/5, x, y, radius, radius, x+radius, y, x, y);
}

//Inside function returns the amount of walls between one point in a ray's direction
int inside(Vec2D pos, Ray2D r) {
  int count = 0;

  //Test all segments and count those that return an intersect
  for(auto s : segs) {
    pair<double, double> result = intersect_seg_ray(s, r);
    double lambda = result.first, dist = result.second;
    if (lambda >= 0 && lambda <= 1 && dist > 0) {
      count++;
    }
  }

  return count;
}


//Calculate Cory's line of sight rays
void lineOfSight(Vec2D pos) {

  //Test all angles
  for (double a = 0.0; a <= (2*PI); a+=0.0025) {

    //Reset rays, and comparative variables
    Ray2D r(pos, Vec2D(cos(a), sin(a)));
    pair<double, double> closest = make_pair(-1, 999999);
    double lineOfSight = 1000.0;
    bool intersects = false;

    //Test the ray with each wall segment
    for (auto s : segs) {
      pair<double, double> result = intersect_seg_ray(s, r);
      double lambda = result.first, dist = result.second;

      //If it intersects, then reselt max dist from cory to the wall
      if (lambda >= 0 && lambda <= 1 && dist > 0) {
        intersects = true;
        if (dist < closest.second) { closest = result; }
      }
    }

    //If no intersect at all, set the dist to "infinite" dist
    if (closest.second == 999999) {
      intersects = true;
      closest.second = 1000.0; }

    //Draw cory's lines of sight
    if (intersects) {
      Vec2D isect_pt = pos + Vec2D(cos(a), sin(a)) * closest.second;
      //circle(10*isect_pt.x, -10*isect_pt.y, 25, 1, 80);
      //line(10*pos.x, -10*pos.y, 10*isect_pt.x, -10*isect_pt.y, 2, 1);

      segs.push_back(Segment2D(Vec2D(pos.x, pos.y), Vec2D(isect_pt.x, isect_pt.y)));
    }
  }
}

void neighbors() {

  //Reiterate over every combination of each building's edge
  for(int i = 0; i < num_vertices; i++) {
    for(int j = 0; j < num_vertices; j++) {

      double abSeg;
      bool flag = false;
      Vec2D a(all[i].first, all[i].second);
      Vec2D b(all[j].first, all[j].second);
      //Only follow through if they are NOT the same building
      if (buildings.at(i) != buildings.at(j)) {

        Ray2D r(a, b-a);
        abSeg = sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));

        //If no walls separate a and b
        if (inside(a, r) == 0) {

          //Test against cory's
          for (auto s : segs) {
            pair<double, double> result = intersect_seg_ray(s, r);
            double lambda = result.first, dist = result.second;

            //If it intersects, then reselt max dist from cory to the wall
            if (lambda >= 0 && lambda <= 1 && dist > 0 && dist < abSeg) {
              flag = true;
            }
          }
        }
      }

      //If passed the conditions, push back int cost and nbrs
      if (!flag) {
        nbrs[a].push_back(b);
        cost[Segment2D(a,b)] = abSeg;
      }
    }
  }
}



//Create the visibility graph for my_pos using Dijkstra's Algorithm
void visibilityGraph() {

  //Start at my_pos
  Vec2D j, i = my_pos;
  Q.push(make_pair(0.0, i));

  while(!Q.empty()) {

    //Chose the next
    i = Q.top().second;
    Q.pop();

    //Find yet-unvisited Vec2D i having min label cost[i];
    if (visited[i] == 0) {

      //Set to visited
      visited[i] = 1;

      //For all edges (i, j) outgoing from i
      for (auto j : nbrs[i]) {

        if (cost[Segment2D(i,j)] < cost[Segment2D(j, mcadams)]) {

          //Current cost += cost of most beneficial neighbor
          //cost[j] = cost[i] + travel_time[make_pair(i,j)];

          path.push_back(Segment2D(i, j));
          Q.push(make_pair(cost[Segment2D(i,j)], j));
        }
      }
    }
  }
}

//DRAW BUILDINGS
void draw_buildings(void) {

  double s = 10.0; // scale factor so line width of 1 pixel is 10 cm, not 1 meter

  xfig_header();

  ifstream places ("campus_places.txt");
  int n, v, build = 0, num = 0, numVertices;
  string name;
  double x, y;
  while (places >> name >> n) {
    vector<pair<double, double>> coords;
    for (int i=0; i<n; i++) {
      places >> x >> y >> v;
      coords.push_back(make_pair(x,y));
      all.push_back(make_pair(x, y));
      buildings.push_back(build);
      num++;
      numVertices++;
    }
    for (int i=0; i<n; i++) {
      line(s*coords[i].first, -s*coords[i].second, s*coords[(i+1)%n].first, -s*coords[(i+1)%n].second, 2, 1);
      segs.push_back(Segment2D(Vec2D(coords[i].first, coords[i].second), Vec2D(coords[(i+1)%n].first, coords[(i+1)%n].second)));
      circle(s*coords[i].first, -s*coords[i].second, 25, 1, 80);
    }
    build++;
  }
}

//MAIN
int main(void) {

  //Read in ending, starting, and Cory's location
  ifstream people("input.txt");
  int x, y;

  people >> x >> y;
  Vec2D mcadams = Vec2D(x, y);
  people >> x >> y;
  Vec2D my_pos = Vec2D(x, y);
  people >> x >> y;
  Vec2D cory = Vec2D(x, y);

  people.close();

  //If starting is not inside a building, then move to ending without being seen by Cory
  double angle = drand48()*2*PI;
  Ray2D r(my_pos, Vec2D(cos(angle), sin(angle)));
  if (inside(my_pos, r)%2 == 0) {
    draw_buildings();
    lineOfSight(cory);
    neighbors();
    visibilityGraph();

    //Draw the found pathway
    for (auto s : path) {
      line(10*s.p.x, -10*s.p.y, 10*s.q.x, -10*s.q.y, 2, 1);
      circle(10*s.p.x, -10*s.p.y, 25, 1, 80);
    }

  } else {
    cout << "You're inside a building." << endl;
  }

  //To do visability graph, do double for loop on every graph (making sure they're
// not in the same building) then see if cory's point and any of his segments crosses the neighbor (if inside returns 0)
// then push back the neighbor. Use dikrjas to find shortest pathway

  return 0;
}
