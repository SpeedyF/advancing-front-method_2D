/** 
 * Copyright (C) 2014 Guangmu Zhu <guangmuzhu@gmail.com>
 *
 *  This file is part of Advancing-Front-Method_2D.
 *
 *  Advancing-Front-Method_2D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Advancing-Front-Method_2D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * VET.hpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#ifndef VET_HPP_
#define VET_HPP_

#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>

struct Point {
	double x;
	double y;

	Point() :
			x(0), y(0) {
	}

	Point(double x, double y) :
			x(x), y(y) {
	}

	Point operator+(const Point& _p) const {
		return Point(x + _p.x, y + _p.y);
	}

	Point operator-(const Point& _p) const {
		return Point(x - _p.x, y - _p.y);
	}

	bool operator==(const Point &_p) const {
		return (x == _p.x && y == _p.y) ? true : false;
	}

	bool operator!=(const Point &_p) const {
		return (x == _p.x && y == _p.y) ? false : true;
	}

	std::string to_string() const {
		return to_string("[", "]", " ");
	}

	std::string to_string(std::string _prefix, std::string _suffix, std::string _delim) const {
		std::stringstream _ret;
		_ret << _prefix << x << _delim << y << _suffix;
		return _ret.str();
	}

	double distance(const Point &_p) const {
		return std::sqrt(std::pow(x - _p.x, 2) + std::pow(y - _p.y, 2));
	}
};

struct Edge {
	Point a;
	Point b;

	Edge() : a(), b() {}

	Edge(const Point &a, const Point&b) :
			a(a), b(b) {
	}
};

struct Triangle {
	Point a;
	Point b;
	Point c;

	Triangle() : a(), b(), c() {}

	Triangle(const Point &a, const Point &b, const Point &c) :
			a(a), b(b), c(c) {
	}
};

struct IPoint: Point {
	unsigned index;

	IPoint() :
			Point(), index(0) {
	}

	IPoint(double x, double y, unsigned index) :
			Point(x, y), index(index) {
	}
};

struct IEdge: Edge {
	IPoint a;
	IPoint b;

	IEdge(const IPoint &a, const IPoint&b) :
			Edge(a, b), a(a), b(b) {
	}
};

struct ITriangle: Triangle {
	IPoint a;
	IPoint b;
	IPoint c;

	ITriangle(const IPoint &a, const IPoint &b, const IPoint &c) :
			Triangle(a, b, c), a(a), b(b), c(c) {
	}
};

#endif /* VET_HPP_ */
