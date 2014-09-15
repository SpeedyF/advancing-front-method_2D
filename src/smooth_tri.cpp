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
 * smooth_tri.cpp
 *
 *  Created on: May 26, 2013
 *      Author: guangmu
 */

#include <algorithm>

#include "MAFM.hpp"

using namespace std;

pair<matrix, double> smooth_triangles(matrix _IAVs, unsigned _cIVs,
		const matrix &_TRI, double _rf, double _VStep, double _CC, double _EC) {
	vector<unsigned> _VVs[_IAVs.get_size().first];
	for (unsigned i = 1; i <= _TRI.get_size().first; ++i) {
		_VVs[static_cast<unsigned>(_TRI(i, 1)) - 1].push_back(_TRI(i, 2));
		_VVs[static_cast<unsigned>(_TRI(i, 1)) - 1].push_back(_TRI(i, 3));
		_VVs[static_cast<unsigned>(_TRI(i, 2)) - 1].push_back(_TRI(i, 1));
		_VVs[static_cast<unsigned>(_TRI(i, 2)) - 1].push_back(_TRI(i, 3));
		_VVs[static_cast<unsigned>(_TRI(i, 3)) - 1].push_back(_TRI(i, 1));
		_VVs[static_cast<unsigned>(_TRI(i, 3)) - 1].push_back(_TRI(i, 2));
	}
	vector<unsigned>::iterator _end_unique;
	for (unsigned i = 0; i < _cIVs; ++i) {
		sort(_VVs[i].begin(), _VVs[i].end());
		_end_unique = unique(_VVs[i].begin(), _VVs[i].end());
		_VVs[i].erase(_end_unique, _VVs[i].end());
	}
	double _err1 = 2 * _VStep, _err2 = _VStep;
	double _tmp1, _tmp2;
	vector<unsigned> _tmp;
	while (_err1 - _err2 >= _VStep * _CC || _err2 > _VStep * _EC) {
		_err1 = _err2;
		_err2 = 0;
		for (unsigned i = 0; i < _cIVs; ++i) {
			_tmp = _VVs[i];
			_tmp1 = _tmp2 = 0;
			for (auto &_ttmp : _tmp) {
				_tmp1 += _IAVs(_ttmp, 1) - _IAVs(i + 1, 1);
				_tmp2 += _IAVs(_ttmp, 2) - _IAVs(i + 1, 2);
			}
			_err2 = fmax(_err2, sqrt(pow(_tmp1, 2) + pow(_tmp2, 2)));
			_IAVs(i + 1, 1) = _IAVs(i + 1, 1) + _rf / _tmp.size() * _tmp1;
			_IAVs(i + 1, 2) = _IAVs(i + 1, 2) + _rf / _tmp.size() * _tmp2;
		}
	}
	return make_pair(_IAVs, _err2 / _VStep);
}
