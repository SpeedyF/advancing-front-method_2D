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
 * interpolation.cpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#include <cmath>
#include <algorithm>

#include "MAFM.hpp"

using namespace std;

namespace {
matrix linspace(const Point &_p1, const Point &_p2, double _VStep) {
	double _dc;
	unsigned _uc;
	double _tx, _ty;
	_dc = _p1.distance(_p2) / _VStep; // 10% error
	if (_dc > 4.5) {
		_uc = round(_dc);
		matrix _ret(_uc - 1, 2);
		_tx = (_p2.x - _p1.x) / _uc;
		_ty = (_p2.y - _p1.y) / _uc;
		for (unsigned i = 1; i < _uc; ++i) {
			_ret.assign_row(i,
					matrix(1, 2, { _p1.x + i * _tx, _p1.y + i * _ty }));
		}
		return move(_ret);
	} else if (_dc >= 1.8) {
		if (_dc < 2.7) {
			matrix _ret(1, 2);
			_ret.assign_row(1,
					matrix(1, 2, { (_p1.x + _p2.x) / 2, (_p1.y + _p2.y) / 2 }));
			return move(_ret);
		} else if (_dc < 3.6) {
			_uc = 3;
			matrix _ret(_uc - 1, 2);
			_tx = (_p2.x - _p1.x) / _uc;
			_ty = (_p2.y - _p1.y) / _uc;
			for (unsigned i = 1; i < _uc; ++i) {
				_ret.assign_row(i,
						matrix(1, 2, { _p1.x + i * _tx, _p1.y + i * _ty }));
			}
			return move(_ret);
		} else {
			_uc = 4;
			matrix _ret(_uc - 1, 2);
			_tx = (_p2.x - _p1.x) / _uc;
			_ty = (_p2.y - _p1.y) / _uc;
			for (unsigned i = 1; i < _uc; ++i) {
				_ret.assign_row(i,
						matrix(1, 2, { _p1.x + i * _tx, _p1.y + i * _ty }));
			}
			return move(_ret);
		}
	} else {
		return matrix(0, 2);
	}
}
}

vector<matrix> boundary_interpolation(
		const std::vector<std::vector<Point>>& _PVs, double _VStep) {
	vector<matrix> _ret;
	for (auto _iter1 = _PVs.cbegin(); _iter1 != _PVs.cend(); ++_iter1) {
		matrix _mtx(0, 2);
		for (auto _iter2 = _iter1->cbegin(), _iter3 = ++_iter1->cbegin();
				_iter3 != _iter1->cend(); ++_iter2, ++_iter3) {
			_mtx ^= matrix(1, 2, { _iter2->x, _iter2->y });
			_mtx ^= linspace(*_iter2, *_iter3, _VStep);
		}
		_mtx ^= matrix(1, 2, { (--_iter1->cend())->x, (--_iter1->cend())->y });
		_mtx ^= linspace(*--_iter1->cend(), *_iter1->cbegin(), _VStep);
		_ret.push_back(_mtx);
	}
	return move(_ret);
}

matrix inner_interpolation(const std::vector<std::vector<Point>>& _PVs,
		const matrix& _BMXY, double _VStep) {
	matrix _LYs = linspace_st(_BMXY(2, 1), _BMXY(2, 2), _VStep, true);
	matrix _ret(0, 2);
	Point _tp;
	vector<vector<Point>>::const_iterator _iter1;
	vector<Point>::const_iterator _iter, _iter2, _iter3;
	vector<Point> _tps;
	for (unsigned i = 1; i <= _LYs.get_length(); ++i) {
		_tps.clear();
		for (_iter1 = _PVs.cbegin(); _iter1 != _PVs.cend(); ++_iter1) {
			for (_iter2 = _iter1->cbegin(), _iter3 = ++_iter1->cbegin();
					_iter3 != _iter1->cend(); ++_iter2, ++_iter3) {
				if (segCrs(Edge(*_iter2, *_iter3),
						Edge(Point(_BMXY(1, 1) - 1, _LYs(1, i)),
								Point(_BMXY(1, 2) + 1, _LYs(1, i))), _tp)) {
					_tp.y = _LYs(1, i);
					_tps.push_back(_tp);
				}
				if (_iter3->y == _LYs(1, i)) {
					_iter = _iter3;
					if (++_iter == _iter1->cend()) {
						_iter = _iter1->cbegin();
					}
					if ((_iter2->y > _iter3->y && _iter3->y > _iter->y)
							|| (_iter2->y < _iter3->y && _iter3->y < _iter->y)) {
						_tps.push_back(*_iter3);
					}
				}
			}
			_iter3 = _iter1->cbegin();
			if (segCrs(Edge(*_iter2, *_iter3),
					Edge(Point(_BMXY(1, 1) - 1, _LYs(1, i)),
							Point(_BMXY(1, 2) + 1, _LYs(1, i))), _tp)) {
				_tp.y = _LYs(1, i);
				_tps.push_back(_tp);
			}
			if (_iter3->y == _LYs(1, i)) {
				_iter = _iter3;
				++_iter;
				if ((_iter2->y > _iter3->y && _iter3->y > _iter->y)
						|| (_iter2->y < _iter3->y && _iter3->y < _iter->y)) {
					_tps.push_back(*_iter3);
				}
			}
		}
		sort(_tps.begin(), _tps.end(),
				[](const Point &_p1, const Point &_p2) {return _p1.x < _p2.x;});
		for (_iter2 = _tps.cbegin(), _iter3 = ++_tps.cbegin();
				_iter2 != _tps.cend() && _iter3 != _tps.cend();
				++++_iter2, ++++_iter3) {
			_ret ^= linspace(*_iter2, *_iter3, _VStep);
		}
	}
	return move(_ret);
}

matrix filter_inner_interpolation(const std::vector<matrix> &_BVs,
		const matrix &_IVs, double _VStep) {
	matrix _ret(_IVs);
	bool _flag;
	for (unsigned i = 1; i <= _ret.get_size().first; ++i) {
		_flag = true;
		for (auto _iter1 = _BVs.cbegin(); _iter1 != _BVs.cend() && _flag;
				++_iter1) {
			for (unsigned j = 1; j <= _iter1->get_size().first; ++j) {
				if (Point(_ret(i, 1), _ret(i, 2)).distance(
						Point((*_iter1)(j, 1), (*_iter1)(j, 2)))
						< 0.9 * _VStep - DERR) {
					_ret ^= i;
					--i;
					_flag = false;
					break;
				}
			}
		}
	}
	return move(_ret);
}
