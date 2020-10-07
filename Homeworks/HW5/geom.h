#define GEOM_H

using namespace std;

struct Vec2D {
  double x, y;
  Vec2D() {x = y = 0; }
  Vec2D(double _x, double _y) {x = _x; y = _y; }
};

struct Segment2D {
  Vec2D p, q;
  Segment2D() { p = Vec2D(); q = Vec2D(); }
  Segment2D(Vec2D _p, Vec2D _q) { p = _p; q = _q; }
};

struct Ray2D {
  Vec2D base, dir;
  Ray2D() { base = Vec2D(); dir = Vec2D(1, 0); }
  Ray2D(Vec2D _base, Vec2D _dir) { base = _base; dir = _dir; }
};

typedef pair<double, int> pdi;

static Vec2D operator+(Vec2D v1, Vec2D v2) { return Vec2D(v1.x+v2.x, v1.y+v2.y); }
static Vec2D operator-(Vec2D v1, Vec2D v2) { return Vec2D(v1.x-v2.x, v1.y-v2.y); }
static Vec2D operator*(Vec2D v, double a) { return Vec2D(v.x*a, v.y*a); }
static Vec2D operator*(double a, Vec2D v) { return Vec2D(a*v.x, a*v.y); }
static Vec2D operator/(Vec2D v, double a) { return Vec2D(v.x/a, v.y/a); }

pair<double,double> intersect_seg_ray(Segment2D s, Ray2D r);
