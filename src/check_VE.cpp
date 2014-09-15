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
 * check_VE.cpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#include <iostream>
#include <algorithm>

#include "MAFM.hpp"

using namespace std;

namespace {
	/*** help function ***/

	template<typename FWIter> bool __check_direction(const FWIter &_bg, const FWIter &_ed) {
		double _angle = 0, _tmpagl = 0;
		FWIter _tmp1, _tmp2, _tmp3;
		for (_tmp1 = _tmp2 = _tmp3 = _bg, ++_tmp2, ++++_tmp3; _tmp3 != _ed;
				++_tmp1, ++_tmp2, ++_tmp3) {
			_tmpagl = mmod(cart2pol(*_tmp3 - *_tmp2)- cart2pol(*_tmp2 - *_tmp1), 2 * M_PI);
			_angle += (mmod(_tmpagl, 2 * M_PI) > M_PI) ? _tmpagl - 2 * M_PI : _tmpagl;
		}
		_tmpagl = mmod(cart2pol(*_bg - *_tmp2) - cart2pol(*_tmp2 - *_tmp1), 2 * M_PI);
		_angle += (mmod(_tmpagl, 2 * M_PI) > M_PI) ? _tmpagl - 2 * M_PI : _tmpagl;
		return _angle > 0;
	}
}

bool check_off(vector<vector<Point>>& _PVs) {
	if (_is_inner_outof_outer(_PVs) || _has_intersecting_lines(_PVs)) {
		return false;
	} else {
		_check_direction(_PVs);
		return true;
	}
}

bool _is_inner_outof_outer(const vector<vector<Point>>& _PVs) {
	for (auto _iter = ++_PVs.cbegin(); _iter != _PVs.end(); ++_iter) {
		if (!iopolygon(_PVs.front().cbegin(), _PVs.front().end(), _iter->begin(),_iter->end()))
			return true;
	}
	return false;
}

bool _has_intersecting_lines(const vector<vector<Point>>& _PVs) {
	vector<Edge> _PEs;
	vector<Point>::const_iterator _tmp;
	for (auto _iter1 = _PVs.cbegin(); _iter1 != _PVs.cend(); ++_iter1) {
		for (auto _iter2 = _iter1->cbegin(); _iter2 != _iter1->cend(); ++_iter2) {
			if (_tmp = _iter2, ++_tmp, _tmp == _iter1->cend()) {
				_PEs.push_back(Edge(*_iter2, *_iter1->begin()));
				continue;
			}
			_PEs.push_back(Edge(*_iter2, *_tmp));
		}
	}
	for (auto _iter1 = _PEs.cbegin(); _iter1 != _PEs.cend(); ++_iter1) {
		for (auto _iter2 = _iter1; _iter2 != _PEs.cend(); ++_iter2) {
			if (_iter1 == _iter2)
				continue;
			if (segCrs(*_iter1, *_iter2))
				return true;
		}
	}
	return false;
}

void _check_direction(vector<vector<Point>>& _PVs) {
	if (!__check_direction(_PVs.cbegin()->cbegin(), _PVs.cbegin()->cend())) {
		cerr << "Outer boundary has negative direction, fixed. " << endl;
		reverse(_PVs.begin()->begin(), _PVs.begin()->end());
	}
	for (auto _iter = ++_PVs.begin(); _iter != _PVs.end(); ++_iter) {
		if (__check_direction(_iter->cbegin(), _iter->cend())) {
			cerr << "Inner boundary has positive direction, fixed. " << endl;
			reverse(_iter->begin(), _iter->end());
		}
	}
}
