#include <utility>
#include <cmath>
#include <iostream>
#include "geom.h"

using namespace std;

// return pair of (lambda, dist)
// lambda tells where on the segment the ray hits (0 = left side ... 1 = right side)
// outside the range 0...1 means it didn't hit the segment
// dist is the distance along the ray to the intersection point
pair<double,double> intersect_seg_ray(Segment2D s, Ray2D r) {
  double len = sqrt(r.dir.x*r.dir.x + r.dir.y*r.dir.y);
  r.dir = r.dir / len;Vec2D segdir = s.p - s.q;
  double cross_product = r.dir.x * segdir.y - r.dir.y * segdir.x;
  if (fabs(cross_product) < 0.0001) return make_pair(-1,-1);
  // parallel, so no intersection
  double num = s.p.x*r.dir.y-r.base.x*r.dir.y-s.p.y*r.dir.x+r.base.y*r.dir.x;
  double den = -s.q.x*r.dir.y+s.p.x*r.dir.y-s.p.y*r.dir.x+s.q.y*r.dir.x;
  double lambda = num / den, dist;
  if (fabs(r.dir.x) > fabs(r.dir.y)) // use whichever of x or y farthest from division by 0
    dist = ((1-lambda)*s.p.x + lambda*s.q.x - r.base.x) / r.dir.x;
  else dist = ((1-lambda)*s.p.y + lambda*s.q.y - r.base.y) / r.dir.y;
  return make_pair(lambda, dist);
}
