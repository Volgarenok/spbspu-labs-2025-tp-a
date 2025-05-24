#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <string>

struct Point {
    int x;
    int y;
    Point(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}
};

struct Polygon {
    std::vector<Point> points;
};

Point operator-(const Point& a, const Point& b);
int dot(const Point& a, const Point& b);
int cross(const Point& a, const Point& b);

double compute_area(const Polygon& poly);
bool is_rectangle(const Polygon& poly);
long long orient(const Point& p, const Point& q, const Point& r);
bool on_segment(const Point& p, const Point& q, const Point& r);
bool seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);
bool point_in_polygon(const Point& pt, const Polygon& poly);
bool polygons_intersect(const Polygon& a, const Polygon& b);
bool parse_polygon(const std::string& str, Polygon& poly);

void process_commands(std::vector<Polygon>& polygons);

#endif
