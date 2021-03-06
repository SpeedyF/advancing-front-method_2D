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
 * matlab_fun.cpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#include "matlab_fun.hpp"

using namespace std;

bool is_zero(double _d) {
	return _d > -DERR && _d < DERR;
}

double det2(const matrix &_mtx) {
	if (!_mtx.is_square_matrix() || _mtx.get_length() != 2)
		throw std::invalid_argument("det only for 2-order matrix");
	return _mtx(1, 1) * _mtx(2, 2) - _mtx(1, 2) * _mtx(2, 1);
}

matrix inv2(const matrix &_mtx) {
	if (!_mtx.is_square_matrix() || _mtx.get_length() != 2)
		throw std::invalid_argument("det only for 2-order matrix");
	if (is_zero(det2(_mtx)))
		throw std::invalid_argument("det equals zero");
	return matrix(2, 2, {_mtx(2, 2), -_mtx(1, 2), -_mtx(2, 1), _mtx(1, 1)}) / det2(_mtx);
}

double det2(const Edge &_e1, const Edge &_e2) {
	Point _p1 = _e1.b - _e1.a;
	Point _p2 = _e2.b - _e2.a;
	return _p1.x * _p2.y - _p2.x * _p1.y;
}

bool is_leftside(const Point &_p1, const Point &_p2, const Point &_p3) {
	if (_p1 == _p3 || _p2 == _p3)
		return false;
	if ((_p1.y - _p2.y) * _p3.x - (_p1.x - _p2.x) * _p3.y
			+ (_p1.x * _p2.y - _p2.x * _p1.y) > DERR)
		return true;
	else
		return false;
}

double mmod(double _d1, double _d2) {
	if (_d2 <= 0)
		return _d1;
	if (_d1 == 0) {
		return 0;
	} else if (_d1 > 0) {
		return fmod(_d1, _d2);
	} else {
		return _d2 -fmod(-_d1, _d2);
	}
}

matrix linspace(int _bg, int _ed) {
	if (_bg == _ed)
		return matrix(1, 0);
	int _step = (_ed > _bg) ? 1 : -1;
	int _count = (_ed > _bg) ? _ed - _bg - 1 : _bg - _ed - 1;
	matrix _ret(1, _count);
	for (int i = 1; i <= _count; ++i) {
		_ret(1, i) = _bg + i * _step;
	}
	return move(_ret);
}

matrix linspace_st(double _bg, double _ed, double _step, bool _round) {
	double _dc = (_ed - _bg) / _step;
	if (_dc < 0)
		return matrix(1, 0);
	if (_round) {
		_dc = round(_dc);
		_step = (_ed - _bg) / _dc;
	}
	unsigned _uc = _dc;
	matrix _ret(1, _uc - 1);
	for (unsigned i = 1; i < _uc; ++i) {
		_ret(1, i) = _bg + i * _step;
	}
	return move(_ret);
}

matrix linspace_ct(double _bg, double _ed, unsigned _count) {
	double _step = (_ed - _bg) / (_count + 1);
	matrix _ret(1, _count);
	for (unsigned i = 1; i <= _count; ++i) {
		_ret(1, i) = _bg + i * _step;
	}
	return move(_ret);
}

double cart2pol(const Point &_p) {
	double _x = _p.x;
	double _y = _p.y;
	double _length = sqrt(pow(_x, 2) + pow(_y, 2));
	double _angle;
	double _anglec = acos(_x / _length);
	double _angles = asin(_y / _length);
	if (_angles > 0) {
		_angle = _anglec;
	} else {
		_angle = 2 * M_PI - _anglec;
	}
	return _angle;
}

double cart2pol(double _x, double _y) {
	double _length = sqrt(pow(_x, 2) + pow(_y, 2));
	double _angle;
	double _anglec = acos(_x / _length);
	double _angles = asin(_y / _length);
	if (_angles > 0) {
		_angle = _anglec;
	} else {
		_angle = 2 * M_PI - _anglec;
	}
	return _angle;
}

void cart2pol(const Point &_p, double &_angle, double &_length) {
	double _x = _p.x;
	double _y = _p.y;
	_length = sqrt(pow(_x, 2) + pow(_y, 2));
	double _anglec = acos(_x / _length);
	double _angles = asin(_y / _length);
	if (_angles > 0) {
		_angle = _anglec;
	} else {
		_angle = 2 * M_PI - _anglec;
	}
	return;
}

void cart2pol(double _x, double _y, double &_angle, double &_length) {
	_length = sqrt(pow(_x, 2) + pow(_y, 2));
	double _anglec = acos(_x / _length);
	double _angles = asin(_y / _length);
	if (_angles > 0) {
		_angle = _anglec;
	} else {
		_angle = 2 * M_PI - _anglec;
	}
	return;
}

bool segCrs(const Edge &_e1, const Edge &_e2) {
	if (is_zero(det2(_e1, _e2)))
		return false;
	matrix _ret = matrix (2, 1, {_e1.a.y - _e2.a.y,
								_e1.a.x - _e2.a.x})
										/
										matrix (2, 2, {_e1.a.y - _e1.b.y, _e2.b.y -_e2.a.y,
														_e1.a.x - _e1.b.x, _e2.b.x - _e2.a.x});
	return _ret(1, 1) > DERR && _ret(1, 1) < 1 - DERR && _ret(2, 1) > DERR && _ret(2, 1) < 1 - DERR;
}

bool segCrs(const Edge &_e1, const Edge &_e2, Point &_p) {
	if (is_zero(det2(_e1, _e2)))
		return false;
	matrix _ret = matrix (2, 1, {_e1.a.y - _e2.a.y,
									_e1.a.x - _e2.a.x})
											/
											matrix (2, 2, {_e1.a.y - _e1.b.y, _e2.b.y -_e2.a.y,
															_e1.a.x - _e1.b.x, _e2.b.x - _e2.a.x});
	_p.x = (_e1.b.x - _e1.a.x) * _ret(1, 1) + _e1.a.x;
	_p.y = (_e1.b.y - _e1.a.y) * _ret(1, 1) + _e1.a.y;
	return _ret(1, 1) > DERR && _ret(1, 1) < 1 - DERR && _ret(2, 1) > DERR && _ret(2, 1) < 1 - DERR;
}
