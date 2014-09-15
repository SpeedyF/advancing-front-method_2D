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
 * create_tri.cpp
 *
 *  Created on: May 23, 2013
 *      Author: guangmu
 */

#include <deque>
#include <cstdio>

#include "MAFM.hpp"

using namespace std;

namespace {
matrix _IAVs;
deque<deque<IEdge>> _AFEs;
matrix _flag;
matrix _TRI(0, 3);

matrix _map; // index->line num

unsigned progress = 0, total, percent = 0;
}

bool _search_segCrs(const Point&, const Point&, const Point&);

unsigned _tri_search(const IPoint&, const IPoint&, double);

unsigned _tri_search(const IEdge&, double);

void _check_edges(deque<IEdge>&);

void _create_triangles(double);

pair<matrix, matrix> create_triangles(const vector<matrix> &_BVs,
		const matrix& _IVs, double _VStep) {
	total = _IVs.get_size().first;

	unsigned _cm = 0;
	vector<matrix>::const_iterator _iter;
	for (_iter = _BVs.cbegin(); _iter != _BVs.cend(); ++_iter) {
		_cm += _iter->get_size().first;
	}
	_IAVs = matrix(_cm + _IVs.get_size().first, 2);
	_IAVs.assign_row(1, _IVs.get_size().first, _IVs);
	for (_cm = _IVs.get_size().first + 1, _iter = _BVs.cbegin();
			_iter != _BVs.cend(); _cm += _iter->get_size().first, ++_iter) {
		_IAVs.assign_row(_cm, _cm + _iter->get_size().first - 1, *_iter);
	}
	_IAVs |= !linspace(0, _IAVs.get_size().first + 1);
	_flag =
			matrix(
					matrix(1, _IVs.get_size().first).init(-1)
							| matrix(1,
									_IAVs.get_size().first
											- _IVs.get_size().first).init(2));
	matrix _IBVs(
			_IAVs(_IVs.get_size().first + 1, _IAVs.get_size().first, "row"));
	_IAVs.sort_row(matrix(1, 2, { 2, 1 }));
	_map = _IAVs(3, "col") | !linspace(0, _IAVs.get_size().first + 1);
	_map.sort_row(matrix(1, 1, { 1 }));
	_map |= 1;
	for (unsigned m = 1, i = 0; i < _BVs.size(); ++i) {
		_AFEs.push_back(deque<IEdge>());
		for (unsigned j = 1; j < _BVs.at(i).get_size().first; ++j) {
			_AFEs.back().push_back(
					IEdge(IPoint(_IBVs(m, 1), _IBVs(m, 2), _IBVs(m, 3)),
							IPoint(_IBVs(m + 1, 1), _IBVs(m + 1, 2),
									_IBVs(m + 1, 3))));
			++m;
		}
		_AFEs.back().push_back(
				IEdge(IPoint(_IBVs(m, 1), _IBVs(m, 2), _IBVs(m, 3)),
						IPoint(_IBVs(m - _BVs.at(i).get_size().first + 1, 1),
								_IBVs(m - _BVs.at(i).get_size().first + 1, 2),
								_IBVs(m - _BVs.at(i).get_size().first + 1,
										3))));
		++m;
	}
	printf("Progress:   0%%...\b\b\b\b");
	fflush(stdout);
	_create_triangles(_VStep);
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\bFinishing.\n");
	fflush(stdout);
	for (unsigned i = 1; i <= _TRI.get_size().first; ++i) {
		for (unsigned j = 1; j <= 3; ++j) {
			_TRI(i, j) = _IAVs(_TRI(i, j), 3);
		}
	}
	_IAVs.sort_row(matrix({1, 1, {3}}));
	return make_pair(_IAVs, _TRI);
}

void _create_triangles(double _VStep) {
	unsigned _count;

	unsigned _index;

	Edge _e1, _e2, _e3, _e4;

	while (true) {
		while (_AFEs.size() > 0 && _AFEs.front().size() == 0)
			_AFEs.pop_front();
		if (_AFEs.size() == 0)
			break;
		for (auto _iter = _AFEs.begin(); _iter != _AFEs.end(); ++_iter) {
			deque<IEdge> &_FEs = *_iter;
			_count = _FEs.size();
			for (unsigned c = 1; c <= _count && _FEs.size() > 0; ++c) {
				_index = _tri_search(_FEs.front(), _VStep);
				_TRI ^= matrix(1, 3,
						{ _map(_FEs.front().a.index, 1), _map(_FEs.front().b.index,
								1), _index }); // line num
				if (_flag(1, _IAVs(_index, 3)) == -1) {
					++progress;
					if (progress * 100 / total > percent) {
						percent = progress * 100 / total;
						printf("\b\b\b%3u", percent);
						fflush(stdout);
					}
				}
				--_flag(1, _FEs.front().a.index);
				--_flag(1, _FEs.front().b.index);
				_FEs.pop_front();
				_check_edges(_FEs);
			}
		}
	}
}

bool _search_segCrs(const Point &_p1, const Point &_p2, const Point &_p3) {
	for (auto _iter1 = _AFEs.cbegin(); _iter1 != _AFEs.cend(); ++_iter1) {
		for (auto _iter2 = _iter1->cbegin(); _iter2 != _iter1->cend();
				++_iter2) {
			if (segCrs(Edge(_p1, _p3), *_iter2)
					|| segCrs(Edge(_p2, _p3), *_iter2))
				return true;
		}
	}
	return false;
}

unsigned _tri_search(const IPoint &_p1, const IPoint &_p2, double _VStep) {
	matrix _MXY(2, 2,
			{ fmin(_p1.x, _p2.x) - 2 * _VStep, fmax(_p1.x, _p2.x) + 2 * _VStep,
					fmin(_p1.y, _p2.y) - 2 * _VStep, fmax(_p1.y, _p2.y)
							+ 2 * _VStep });
	unsigned _index, _ret = 0;
	for (_index = 1;
			_index <= _IAVs.get_size().first && _IAVs(_index, 2) < _MXY(2, 1);
			++_index)
		;
	double _min_dist = 2 * pow((3 * _VStep), 2), _curr_dist;
	while (_index <= _IAVs.get_size().first && _IAVs(_index, 2) <= _MXY(2, 2)) {
		if (_IAVs(_index, 1) >= _MXY(1, 1) && _IAVs(_index, 1) <= _MXY(1, 2)
				&& _flag(1, _IAVs(_index, 3)) != 0
				&& !_search_segCrs(_p1, _p2,
						Point(_IAVs(_index, 1), _IAVs(_index, 2)))
				&& is_leftside(_p1, _p2,
						IPoint(_IAVs(_index, 1), _IAVs(_index, 2),
								_IAVs(_index, 3)))) {
			_curr_dist = pow(_IAVs(_index, 1) - _p1.x, 2)
					+ pow(_IAVs(_index, 2) - _p1.y, 2)
					+ pow(_IAVs(_index, 1) - _p2.x, 2)
					+ pow(_IAVs(_index, 2) - _p2.y, 2);
			if (_min_dist >= _curr_dist) {
				_min_dist = _curr_dist;
				_ret = _index;
			}
		}
		++_index;
	}
	return _ret;
}

unsigned _tri_search(const IEdge &_eg, double _VStep) {
	return _tri_search(_eg.a, _eg.b, _VStep);
}

void _check_edges(deque<IEdge>& _FEs) {
	bool _found[2] { false, false };
	unsigned _last_TRi = _TRI.get_size().first;
	for (auto _iter1 = _AFEs.begin(); _iter1 != _AFEs.end(); ++_iter1) {
		for (auto _iter2 = _iter1->begin(); _iter2 != _iter1->end(); ++_iter2) {
			if ((_map(_iter2->a.index, 1) == _TRI(_last_TRi, 1)
					&& _map(_iter2->b.index, 1) == _TRI(_last_TRi, 3))
					|| (_map(_iter2->a.index, 1) == _TRI(_last_TRi, 3)
							&& _map(_iter2->b.index, 1) == _TRI(_last_TRi, 1))) {
				--_flag(1, _iter2->a.index);
				--_flag(1, _iter2->b.index);
				_iter2 = _iter1->erase(_iter2);
				--_iter2;
				_found[0] = true;
				continue;
			}
			if ((_map(_iter2->a.index, 1) == _TRI(_last_TRi, 2)
					&& _map(_iter2->b.index, 1) == _TRI(_last_TRi, 3))
					|| (_map(_iter2->a.index, 1) == _TRI(_last_TRi, 3)
							&& _map(_iter2->b.index, 1) == _TRI(_last_TRi, 2))) {
				--_flag(1, _iter2->a.index);
				--_flag(1, _iter2->b.index);
				_iter2 = _iter1->erase(_iter2);
				--_iter2;
				_found[1] = true;
				continue;
			}
		}
	}
	if (_found[0] == 0) {
		_FEs.push_back(
				IEdge(
						IPoint(_IAVs(_TRI(_last_TRi, 1), 1),
								_IAVs(_TRI(_last_TRi, 1), 2),
								_IAVs(_TRI(_last_TRi, 1), 3)),
						IPoint(_IAVs(_TRI(_last_TRi, 3), 1),
								_IAVs(_TRI(_last_TRi, 3), 2),
								_IAVs(_TRI(_last_TRi, 3), 3))));
		++_flag(1, _IAVs(_TRI(_last_TRi, 1), 3));
		if (_flag(1, _IAVs(_TRI(_last_TRi, 3), 3)) == -1) {
			_flag(1, _IAVs(_TRI(_last_TRi, 3), 3)) = 1;
		} else {
			++_flag(1, _IAVs(_TRI(_last_TRi, 3), 3));
		}
	}
	if (_found[1] == 0) {
		_FEs.push_back(
				IEdge(
						IPoint(_IAVs(_TRI(_last_TRi, 3), 1),
								_IAVs(_TRI(_last_TRi, 3), 2),
								_IAVs(_TRI(_last_TRi, 3), 3)),
						IPoint(_IAVs(_TRI(_last_TRi, 2), 1),
								_IAVs(_TRI(_last_TRi, 2), 2),
								_IAVs(_TRI(_last_TRi, 2), 3))));
		++_flag(1, _IAVs(_TRI(_last_TRi, 2), 3));
		if (_flag(1, _IAVs(_TRI(_last_TRi, 3), 3)) == -1) {
			_flag(1, _IAVs(_TRI(_last_TRi, 3), 3)) = 1;
		} else {
			++_flag(1, _IAVs(_TRI(_last_TRi, 3), 3));
		}
	}
}
