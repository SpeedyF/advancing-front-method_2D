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
 * format_off.cpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#include <iostream>
#include <sstream>
#include <fstream>

#include "MAFM.hpp"

using namespace std;

namespace {
vector<Point> _APVs;
}

/*
 * _path:	path of ._off file
 * _PVs:		polygon vertexes
 */
bool load_off(const std::string &_path, std::vector<std::vector<Point>> &_PVs) {
	ifstream _off(_path);
	if (!_off) {
		cerr << "File: " << _path << " doesn't exist. " << endl;
		_off.close();
		return false;
	}

	string _line;
	stringstream _sline;
	unsigned _cv = 0; // count of vertexes
	unsigned _cf = 0; // count of faces
	double _x = 0;
	double _y = 0;
	unsigned _count;
	unsigned _index;

	if (getline(_off, _line)) {
		if (_line != "OFF") {
			cerr << "File: " << _path << " isn't _off format. " << endl;
			_off.close();
			return false;
		}
	} else {
		cerr << "File: " << _path << " is broken. " << endl;
		_off.close();
		return false;
	}
	if (getline(_off, _line)) {
		_sline.str(_line);
		_sline >> _cv;
		_sline >> _cf;
		// Regardless of edges
		if (!_sline && !_sline.eof()) {
			cerr << "File: " << _path << " is broken. " << endl;
			_off.close();
			return false;
		}
	} else {
		cerr << "File: " << _path << " is broken. " << endl;
		_off.close();
		return false;
	}
	for (unsigned i = 0; i < _cv; ++i) {
		_sline.clear();
		if (getline(_off, _line)) {
			_sline.str(_line);
			_sline >> _x;
			_sline >> _y;
			_APVs.push_back(Point(_x, _y));
			// Regardless of z
			if (!_sline && !_sline.eof()) {
				cerr << "File: " << _path << " is broken. " << endl;
				_off.close();
				return false;
			}
		} else {
			cerr << "File: " << _path << " is broken. " << endl;
			_off.close();
			return false;
		}
	}
	for (unsigned i = 0; i < _cf; ++i) {
		_sline.clear();
		if (getline(_off, _line)) {
			_sline.str(_line);
			_sline >> _count;
			if (!_sline && !_sline.eof()) {
				cerr << "File: " << _path << " is broken. " << endl;
				_off.close();
				return false;
			}
			if (_count < 3) {
				cerr << "File: " << _path << " is broken. " << endl;
				_off.close();
				return false;
			}
			_PVs.push_back(vector<Point>());
			for (unsigned j = 0; j < _count; ++j) {
				if (_sline >> _index) {
					_PVs.back().push_back(_APVs.at(_index));
				} else {
					cerr << "File: " << _path << " is broken. " << endl;
					_off.close();
					return false;
				}
			}
		} else {
			cerr << "File: " << _path << " is broken. " << endl;
			_off.close();
			return false;
		}
	}
	return true;
}

void save_OFF(const matrix &_MP, const matrix &_MT, const std::string& _Path) {
	ofstream of(_Path, ofstream::trunc);
	of << "OFF\n";
	of << _MP.get_size().first << ' ' << _MT.get_size().first << ' ' << 0
			<< '\n';
	of << _MP.to_string("", "", " ") << '\n';
	of << _MT.to_string("", "", " ") << '\n';
	of.flush();
	of.close();
}
