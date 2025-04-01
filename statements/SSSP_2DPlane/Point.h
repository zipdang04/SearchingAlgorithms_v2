#pragma once

#include <math.h>

struct Point {
	long long x, y;
	public:
		Point() { x = y = 0; }
		Point(long long x, long long y): x(x), y(y) {}
		inline long long selfDot();
		inline double abs();
		static inline double dist(Point a, Point b);
};
inline Point operator + (Point a, Point b) { return {a.x + b.x, a.y + b.y}; }
inline Point operator - (Point a, Point b) { return {a.x - b.x, a.y - b.y}; }
inline Point operator * (Point a, long long b) { return {a.x * b, a.y * b}; }
inline Point operator * (long long b, Point a) { return {a.x * b, a.y * b}; }
inline long long Point::selfDot() { return x*x + y*y; }
inline double Point::abs() { return sqrt(selfDot()); }
inline double Point::dist(Point a, Point b) { return (a-b).abs(); }
