#pragma once

#include <math.h>

struct Point {
	double x, y;
	public:
		Point() { x = y = 0; }
		Point(double x, double y): x(x), y(y) {}
		inline double selfDot();
		inline double abs();
		static inline double dist(Point a, Point b);
};
inline Point operator + (Point a, Point b) { return {a.x + b.x, a.y + b.y}; }
inline Point operator - (Point a, Point b) { return {a.x - b.x, a.y - b.y}; }
inline Point operator * (Point a, double b) { return {a.x * b, a.y * b}; }
inline Point operator * (double b, Point a) { return {a.x * b, a.y * b}; }
inline double Point::selfDot() { return x*x + y*y; }
inline double Point::abs() { return sqrt(selfDot()); }
inline double Point::dist(Point a, Point b) { return (b-a).abs(); }
