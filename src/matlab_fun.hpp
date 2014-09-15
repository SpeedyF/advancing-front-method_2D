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
 * matlab_fun.hpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#ifndef MATLAB_FUN_HPP_
#define MATLAB_FUN_HPP_

#include <cmath>
#include <cstring>
#include <utility>
#include <sstream>
#include <memory>
#include <stdexcept>

#include "VET.hpp"

const double DERR = 2.67e-9;

bool is_zero(double _d);

class matrix;

double det2(const matrix&);

matrix inv2(const matrix&);

double det2(const Edge&, const Edge&);

bool is_leftside(const Point&, const Point&, const Point&);

class matrix {
	unsigned _m = 0;
	unsigned _n = 0;
	double *_dmtx = nullptr;

public:
	matrix() :
			_m(0), _n(0), _dmtx(nullptr) {
	}

	matrix(unsigned _m, unsigned _n) :
			_m(_m), _n(_n) {
		_dmtx = new double[_m * _n]();
	}

	matrix(unsigned _m, unsigned _n, double *_dmtx) :
			_m(_m), _n(_n), _dmtx(_dmtx) {
	}

	matrix(unsigned _m, unsigned _n, const std::initializer_list<double> &_dmtx) :
			_m(_m), _n(_n) {
		if (_dmtx.size() < _m * _n)
			throw std::bad_alloc();
		this->_dmtx = new double[_m * _n]();
		auto __iter = _dmtx.begin();
		for (unsigned i = 0; i < _m * _n; ++i, ++__iter) {
			this->_dmtx[i] = *__iter;
		}
	}

	matrix(const matrix &_mtx) :
			_m(_mtx._m), _n(_mtx._n) {
		if (_dmtx != nullptr)
			delete[] _dmtx;
		_dmtx = new double[_m * _n]();
		if (_mtx._dmtx != nullptr)
			std::memcpy(_dmtx, _mtx._dmtx, _m * _n * sizeof(double));
	}

	matrix(matrix &&_mtx) noexcept : _m(_mtx._m), _n(_mtx._n) {
		if (_dmtx != nullptr)
		delete [] _dmtx;
		if (_mtx._dmtx == nullptr) {
			_dmtx = nullptr;
			return;
		} else {
			_dmtx = _mtx._dmtx;
			_mtx._dmtx = nullptr;
		}
	}

	~matrix() {
		if (_dmtx != nullptr) {
			delete [] _dmtx;
		}
	}

	matrix& init(double _d) {
		for (unsigned i = 0; i < _m * _n; ++i) {
			*(_dmtx + i) = _d;
		}
		return *this;
	}

	matrix& release() {
		if (_dmtx != nullptr)
		_dmtx = nullptr;
		return *this;
	}

	bool is_square_matrix() const {
		return _m == _n;
	}

	unsigned get_length() const {
		return _m > _n ? _m : _n;
	}

	void get_size(unsigned &_m, unsigned &_n) const {
		_m = this->_m;
		_n = this->_n;
	}

	std::pair<unsigned, unsigned> get_size() const {
		return std::make_pair(_m, _n);
	}

	void set_size(unsigned _m, unsigned _n) {
		if (_m * _n != this->_m * this->_n)
		throw std::invalid_argument("Dimension unmatched");
		this->_m = _m;
		this->_n = _n;
	}

	std::string to_string() const {
		return to_string("[", "]", " ");
	}

	std::string to_string(std::string _prefix, std::string _suffix, std::string _delim) const {
		std::stringstream _ret;
		for (unsigned i = 0; i < _m; ++i) {
			_ret << _prefix;
			for (unsigned j = 0; j < _n; ++j) {
				_ret << *(_dmtx + i * _n + j);
				if (j != _n - 1)
				_ret << _delim;
			}
			_ret << _suffix;
			if (i != _m - 1)
			_ret << '\n';
		}
		return std::move(_ret.str());
	}

	matrix& operator=(const matrix &_mtx) {
		_m = _mtx._m;
		_n = _mtx._n;
		if (_dmtx != nullptr)
		delete [] _dmtx;
		_dmtx = new double[_m * _n]();
		if (_mtx._dmtx != nullptr)
		std::memcpy(_dmtx, _mtx._dmtx, _m * _n * sizeof(double));
		return *this;
	}

	matrix& operator=(matrix &&_mtx) noexcept {
		_m = _mtx._m;
		_n = _mtx._n;
		if (_dmtx != nullptr)
		delete [] _dmtx;
		if (_mtx._dmtx == nullptr) {
			_dmtx = nullptr;
		} else {
			_dmtx = _mtx._dmtx;
			_mtx._dmtx = nullptr;
		}
		return *this;
	}

	bool operator==(const matrix &_mtx) const {
		unsigned __m1, __n1, __m2, __n2;
		if (_dmtx == nullptr && _mtx._dmtx == nullptr) {
			return true;
		} else if (_dmtx == nullptr || _mtx._dmtx == nullptr) {
			return false;
		} else if (get_size(__m1, __n1), _mtx.get_size(__m2, __n2), __m1 != __m2 || __n1 != __n2) {
			return false;
		} else {
			return std::memcmp(_dmtx, _mtx._dmtx, _m * _n * sizeof(double)) == 0;
		}
	}

	bool operator!=(const matrix &_mtx) const {
		return !(*this == _mtx);
	}

	double& operator()(unsigned _midx, unsigned _nidx) const {
		if (_midx > 0 &&_midx <= _m && _nidx > 0 && _nidx <= _n) {
			return *(_dmtx + (_midx - 1) * _n + _nidx - 1);
		} else {
			throw std::out_of_range("out of range in matrix: "
					"double& operator()(unsigned _midx, unsigned _nidx)");
		}
	}

	matrix operator()(unsigned _idx, std::string _rc) const {
		if (_rc != "row" && _rc != "col")
			throw std::invalid_argument("row or col");
		if (_rc == "row" && _idx > 0 && _idx <= _m) {
			matrix _ret(1, _n);
			std::memcpy(_ret._dmtx, _dmtx + (_idx - 1) * _n, _n * sizeof(double));
			return std::move(_ret);
		} else if (_rc == "col" && _idx > 0 && _idx <= _n) {
			matrix _ret(_m, 1);
			for (unsigned i = 0; i < _m; ++i) {
				_ret(i + 1, 1) = *(_dmtx + i * _n + _idx - 1);
			}
			return std::move(_ret);
		} else {
			throw std::out_of_range("out of range in matrix: "
					"matrix operator()(unsigned _idx, std::string _rc)");
		}
	}

	matrix operator()(unsigned _idxbg, unsigned _idxed, std::string _rc) const {
		if (_idxbg > _idxed)
			throw std::invalid_argument("error with matrix operator()");
		if (_rc != "row" && _rc != "col")
			throw std::invalid_argument("row or col");
		if (_rc == "row" && _idxbg > 0 && _idxed <= _m) {
			if (_idxbg == _idxed)
				return matrix(0, _n);
			matrix _ret(_idxed - _idxbg + 1, _n);
			std::memcpy(_ret._dmtx, _dmtx + (_idxbg - 1) * _n, (_idxed - _idxbg + 1) * _n * sizeof(double));
			return std::move(_ret);
		} else if (_rc == "col" && _idxbg > 0 && _idxed <= _n) {
			if (_idxbg == _idxed)
				return matrix(_m, 0);
			matrix _ret(_m, _idxed - _idxbg + 1);
			for (unsigned i = 0; i < _m; ++i) {
				for (unsigned j = _idxbg; j <= _idxed; ++j) {
					_ret(i + 1, j - _idxbg + 1) = *(_dmtx + i * _n + j - 1);
				}
			}
			return std::move(_ret);
		} else {
			throw std::out_of_range("out of range in matrix: "
					"matrix operator()(unsigned _idxbg, unsigned _idxed, std::string _rc)");
		}
	}

	matrix operator()(unsigned _rb, unsigned _rd, unsigned _cb, unsigned _cd) const {
		if (_rb > 0 && _rb <= _m && _rd > 0 && _rd <= _m &&
				_cb > 0 && _cb <= _n && _cd > 0 && _cd <= _n) {
			matrix _ret(_rd - _rb + 1, _cd -_cb + 1);
			for (unsigned i = _rb; i <= _rd; ++i) {
				for (unsigned j = _cb; j <= _cd; ++j) {
					_ret(i - _rb + 1, j - _cb + 1) = *(_dmtx + (i - 1) * _n + j - 1);
				}
			}
			return std::move(_ret);
		} else {
			throw std::out_of_range("out of range in matrix: "
					"matrix operator()(unsigned _rb, unsigned _rd, unsigned _cb, unsigned _cd)");
		}
	}

	matrix operator+(double _d) const {
		matrix _ret(*this);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + i * _n + j) += _d;
			}
		}
		return std::move(_ret);
	}

	matrix& operator+=(double _d) {
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_dmtx + i * _n + j) += _d;
			}
		}
		return *this;
	}

	matrix operator-(double _d) const {
		matrix _ret(*this);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + i * _n + j) -= _d;
			}
		}
		return std::move(_ret);
	}

	matrix& operator-=(double _d) {
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_dmtx + i * _n + j) -= _d;
			}
		}
		return *this;
	}

	matrix operator&(const matrix &_mtx) const {
		if (_n != _mtx._m)
			throw std::invalid_argument("col not equal to row");
		matrix _ret(_m, _mtx._n);
		double _tmp;
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _mtx._n; ++j) {
				_tmp = 0;
				for (unsigned k = 0; k < _n; ++k) {
					_tmp += *(_dmtx + i * _n + k) * *(_mtx._dmtx + k * _mtx._n + j);
				}
				*(_ret._dmtx + i * _mtx._n + j) = _tmp;
			}
		}
		return std::move(_ret);
	}

	matrix& operator&=(const matrix &_mtx) {
		if (_n != _mtx._m)
			throw std::invalid_argument("col not equal to row");
		matrix _ret(_m, _mtx._n);
		double _tmp;
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _mtx._n; ++j) {
				_tmp = 0;
				for (unsigned k = 0; k < _n; ++k) {
					_tmp += *(_dmtx + i * _n + k) * *(_mtx._dmtx + k * _mtx._n + j);
				}
				*(_ret._dmtx + i * _mtx._n + j) = _tmp;
			}
		}
		*this = std::move(_ret);
		return *this;
	}

	matrix operator&(double _d) const {
		matrix _ret(*this);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + i * _n + j) *= _d;
			}
		}
		return std::move(_ret);
	}

	matrix& operator&=(double _d) {
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_dmtx + i * _n + j) *= _d;
			}
		}
		return *this;
	}

	matrix operator/(const matrix &_mtx) const {
		return inv2(_mtx) & *this;
	}

	matrix& operator/=(const matrix &_mtx) {
		*this = inv2(_mtx) & *this;
		return *this;
	}

	matrix operator/(double _d) const {
		matrix _ret(*this);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + i * _n + j) /= _d;
			}
		}
		return std::move(_ret);
	}

	matrix& operator/=(double _d) {
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_dmtx + i * _n + j) /= _d;
			}
		}
		return *this;
	}

	matrix operator|(const matrix &_mtx) const {
		if (_m != _mtx._m)
			throw std::invalid_argument("Dimension unmatched");
		matrix _ret(_m, _n + _mtx._n);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + i * (_n + _mtx._n) + j) = *(_dmtx + i * _n + j);
			}
			for (unsigned k = 0; k < _mtx._n; ++k) {
				*(_ret._dmtx + i * (_n + _mtx._n) + _n + k) = *(_mtx._dmtx + i * _mtx._n + k);
			}
		}
		return std::move(_ret);
	}

	matrix& operator|=(const matrix &_mtx) {
		if (_m != _mtx._m)
			throw std::invalid_argument("Dimension unmatched");
		matrix _ret(_m, _n + _mtx._n);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + i * (_n + _mtx._n) + j) = *(_dmtx + i * _n + j);
			}
			for (unsigned k = 0; k < _mtx._n; ++k) {
				*(_ret._dmtx + i * (_n + _mtx._n) + _n + k) = *(_mtx._dmtx + i * _mtx._n + k);
			}
		}
		*this = std::move(_ret);
		return *this;
	}

	matrix operator|(unsigned _idx) const {
		if (_idx < 1 || _idx > _n)
			throw std::out_of_range("out of range in matrix: "
					"matrix operator|(unsigned _idx)");
		matrix _ret(_m, _n -1);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _idx - 1; ++j) {
				*(_ret._dmtx + i * (_n - 1) + j) = *(_dmtx + i * _n + j);
			}
			for (unsigned k = _idx; k < _n; ++k) {
				*(_ret._dmtx + i * (_n - 1) + k - 1) = *(_dmtx + i * _n + k);
			}
		}
		return std::move(_ret);
	}

	matrix& operator|=(unsigned _idx) {
		if (_idx < 1 || _idx > _n)
			throw std::out_of_range("out of range in matrix: "
					"matrix& operator|=(unsigned _idx)");
		matrix _ret(_m, _n -1);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _idx - 1; ++j) {
				*(_ret._dmtx + i * (_n - 1) + j) = *(_dmtx + i * _n + j);
			}
			for (unsigned k = _idx; k < _n; ++k) {
				*(_ret._dmtx + i * (_n - 1) + k - 1) = *(_dmtx + i * _n + k);
			}
		}
		*this = std::move(_ret);
		return *this;
	}

	matrix operator^(const matrix &_mtx) const {
		if (_n != _mtx._n)
			throw std::invalid_argument("Dimension unmatched");
		matrix _ret(_m + _mtx._m, _n);
		std::memcpy(_ret._dmtx, _dmtx, _m * _n * sizeof(double));
		std::memcpy(_ret._dmtx + _m * _n, _mtx._dmtx, _mtx._m * _n * sizeof(double));
		return std::move(_ret);
	}

	matrix& operator^=(const matrix &_mtx) {
		if (_n != _mtx._n)
			throw std::invalid_argument("Dimension unmatched");
		matrix _ret(_m + _mtx._m, _n);
		std::memcpy(_ret._dmtx, _dmtx, _m * _n * sizeof(double));
		std::memcpy(_ret._dmtx + _m * _n, _mtx._dmtx, _mtx._m * _n * sizeof(double));
		*this = std::move(_ret);
		return *this;
	}

	matrix operator^(unsigned _idx) const {
		if (_idx < 1 || _idx > _m)
			throw std::out_of_range("out of range in matrix: "
					"matrix operator^(unsigned _idx)");
		matrix _ret(_m - 1, _n);
		std::memcpy(_ret._dmtx, _dmtx, (_idx - 1) * _n * sizeof(double));
		std::memcpy(_ret._dmtx + (_idx - 1) * _n, _dmtx + _idx * _n, (_m - _idx) * _n * sizeof(double));
		return std::move(_ret);
	}

	matrix& operator^=(unsigned _idx) {
		if (_idx < 1 || _idx > _m)
			throw std::out_of_range("out of range in matrix: "
					"matrix& operator^=(unsigned _idx)");
		matrix _ret(_m - 1, _n);
		std::memcpy(_ret._dmtx, _dmtx, (_idx - 1) * _n * sizeof(double));
		std::memcpy(_ret._dmtx + (_idx - 1) * _n, _dmtx + _idx * _n, (_m - _idx) * _n * sizeof(double));
		*this = std::move(_ret);
		return *this;
	}

	matrix operator~() const {
		if (_m == 1 || _n == 1) {
			matrix _ret(*this);
			_ret.set_size(_n, _m);
			return std::move(_ret);
		}
		matrix _ret(_n, _m);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + j * _m + i) = *(_dmtx + i * _n + j);
			}
		}
		return std::move(_ret);
	}

	matrix& operator!() {
		if (_m == 1 || _n == 1) {
			this->set_size(_n, _m);
			return *this;
		}
		matrix _ret(_n, _m);
		for (unsigned i = 0; i < _m; ++i) {
			for (unsigned j = 0; j < _n; ++j) {
				*(_ret._dmtx + j * _m + i) = *(_dmtx + i * _n + j);
			}
		}
		*this = std::move(_ret);
		return *this;
	}

	void assign_row(unsigned _idx, const matrix &_mtx) {
		if (_mtx.get_size().first < 1)
			throw std::invalid_argument("empty matrix");
		if (_idx < 1 || _idx > _m)
			throw std::out_of_range("out of range in matrix: "
					"void assign_row(unsigned _idx, const matrix &_mtx)");
		if (_n != _mtx._n)
			throw std::invalid_argument("Dimension unmatched");
		std::memcpy(_dmtx + (_idx - 1) * _n, _mtx._dmtx, _n * sizeof(double));
	}

	void assign_row(unsigned _idxbg, unsigned _idxed, const matrix &_mtx) {
		if (_idxbg > _idxed)
			throw std::invalid_argument("error with assign_row");
		if (_mtx.get_size().first < _idxed - _idxbg + 1)
			throw std::invalid_argument("matrix is too small");
		if (_idxbg < 1 || _idxed > _m)
			throw std::out_of_range("out of range in matrix: "
					"void assign_row(unsigned _idxbg, unsigned _idxed, const matrix &_mtx)");
		if (_n != _mtx._n)
			throw std::invalid_argument("Dimension unmatched");
		std::memcpy(_dmtx + (_idxbg - 1) * _n, _mtx._dmtx, (_idxed - _idxbg + 1) * _n * sizeof(double));
	}

	void swap_row(unsigned _idx1, unsigned _idx2) {
		if (_idx1 < 1 || _idx1 > _m || _idx2 < 1 || _idx2 > _m)
			throw std::out_of_range("out of range in matrix: "
					"void swap_row(unsigned _idx1, unsigned _idx2)");
		if (_idx1 == _idx2)
			return;
		matrix _tmp(1, _n);
		std::memcpy(_tmp._dmtx, _dmtx + (_idx1 - 1) * _n, _n * sizeof(double));
		std::memcpy(_dmtx + (_idx1 - 1) * _n, _dmtx + (_idx2 - 1) * _n, _n * sizeof(double));
		std::memcpy(_dmtx + (_idx2 - 1) * _n, _tmp._dmtx, _n * sizeof(double));
	}

	int compare_row(unsigned _ridx1, unsigned _ridx2, const matrix& _cidx) {
		if (_ridx1 < 1 || _ridx1 > _m || _ridx2 < 1 || _ridx2 > _m)
			throw std::out_of_range("out of range in matrix: "
					"int compare_row(unsigned _ridx1, unsigned _ridx2, const matrix& _cidx)");
		if (_cidx.get_size().first < 1 || _cidx.get_size().second < 1)
			throw std::invalid_argument("Dimension unmatched");
		for (unsigned i = 1; i <= _cidx.get_size().second; ++i) {
			if (this->operator()(_ridx1, _cidx(1, i)) == this->operator()(_ridx2, _cidx(1, i)))
				continue;
			return (this->operator()(_ridx1, _cidx(1, i)) < this->operator()(_ridx2, _cidx(1, i)) ? 1 : -1)
					* (_cidx(1, i) > 0 ? 1 : -1);
		}
		return 0;
	}

	void sort_row(const matrix& _cidx) {
		sort_row(1, _m, _cidx);
	}

	void sort_row(unsigned _ridxbg, unsigned _ridxed, const matrix& _cidx) {
		if (_ridxbg < 1 || _ridxbg > _m || _ridxed < 1 || _ridxed > _m)
			throw std::out_of_range("out of range in matrix: "
					"void sort_row(unsigned _ridxbg, unsigned _ridxed, const matrix& _cidx)");
		if (_ridxbg >= _ridxed)
			return;
		if (_cidx.get_size().first < 1 || _cidx.get_size().second < 1)
			throw std::invalid_argument("Dimension unmatched");


		matrix _tmp(1, _n);
		unsigned _bg = _ridxbg, _ed = _ridxed;
		unsigned _key = _bg++;
		while(_bg != _ed) {
			if (compare_row(_bg, _key, _cidx) > 0) {
				++_bg;
			} else {
				while(_bg != _ed && compare_row(_key, _ed, _cidx) > 0) {
					_ed--;
				}
				if (_bg != _ed) {
					std::memcpy(_tmp._dmtx, _dmtx + (_bg - 1) * _n, _n * sizeof(double));
					std::memcpy(_dmtx + (_bg - 1) * _n, _dmtx + (_ed - 1) * _n, _n * sizeof(double));
					std::memcpy(_dmtx + (_ed - 1) * _n, _tmp._dmtx, _n * sizeof(double));
				}
			}
		}

		if (compare_row(_bg, _key, _cidx) < 0)
			--_bg;
		if (_bg != _key) {
			std::memcpy(_tmp._dmtx, _dmtx + (_bg - 1) * _n, _n * sizeof(double));
			std::memcpy(_dmtx + (_bg - 1) * _n, _dmtx + (_key - 1) * _n, _n * sizeof(double));
			std::memcpy(_dmtx + (_key - 1) * _n, _tmp._dmtx, _n * sizeof(double));
		}

		sort_row(_ridxbg, _bg, _cidx);
		sort_row(_ed, _ridxed, _cidx);
	}
};

double mmod(double, double);

matrix linspace(int, int);

matrix linspace_st(double, double, double, bool);

matrix linspace_ct(double, double, unsigned);

double cart2pol(const Point&);

double cart2pol(double, double);

void cart2pol(const Point&, double&, double&);

void cart2pol(double, double, double&, double&);

template<typename FWIter> bool inpolygon(const FWIter&, const FWIter&,
		const FWIter&, const FWIter&);

template<typename FWIter> bool onpolygon(const FWIter&, const FWIter&,
		const FWIter&, const FWIter&);

template<typename FWIter> bool iopolygon(const FWIter&, const FWIter&,
		const FWIter&, const FWIter&);

bool segCrs(const Edge&, const Edge&);

bool segCrs(const Edge&, const Edge&, Point&);

/*** Definitions ***/

template<typename FWIter> bool inpolygon(const FWIter &_Bbg, const FWIter &_Bed,
		const FWIter &_Ibg, const FWIter &_Ied) {
	double _angle, _tmpagl;
	FWIter _tmp1, _tmp2;
	for (FWIter __iter = _Ibg; __iter != _Ied; ++__iter) {
		_angle = 0;
		for (_tmp1 = _Bbg, _tmp2 = _tmp1, ++_tmp2; _tmp2 != _Bed;
				++_tmp1, ++_tmp2) {
			if (*__iter == *_tmp1) {
				return false;
			}
			if (is_zero(
					fabs(
							cart2pol(*_tmp2 - *__iter)
									- cart2pol(*_tmp1 - *__iter)) - M_PI)) {
				return false;
			}
			_tmpagl = mmod(
					cart2pol(*_tmp2 - *__iter) - cart2pol(*_tmp1 - *__iter),
					2 * M_PI);
			_angle +=
					(mmod(_tmpagl, 2 * M_PI) > M_PI) ?
							_tmpagl - 2 * M_PI : _tmpagl;
		}
		_tmpagl = mmod(cart2pol(*_Bbg - *__iter) - cart2pol(*_tmp1 - *__iter),
				2 * M_PI);
		_angle +=
				(mmod(_tmpagl, 2 * M_PI) > M_PI) ? _tmpagl - 2 * M_PI : _tmpagl;
		if (*__iter == *_tmp1 || is_zero(_angle)) {
			return false;
		}
	}
	return true;
}

template<typename FWIter> bool onpolygon(const FWIter &_Bbg, const FWIter &_Bed,
		const FWIter &_Ibg, const FWIter &_Ied) {
	bool _flag;
	FWIter _tmp1, _tmp2;
	for (FWIter __iter = _Ibg; __iter != _Ied; ++__iter) {
		_flag = false;
		for (_tmp1 = _Bbg, _tmp2 = _tmp1, ++_tmp2; _tmp2 != _Bed;
				++_tmp1, ++_tmp2) {
			if (*__iter == *_tmp1) {
				_flag = true;
				break;
			}
			if (is_zero(
					fabs(
							cart2pol(*_tmp2 - *__iter)
									- cart2pol(*_tmp1 - *__iter)) - M_PI)) {
				_flag = true;
				break;
			}
		}
		if (_flag) {
			continue;
		} else {
			if (*__iter != *_tmp1
					&& !is_zero(
							fabs(
									cart2pol(*_Bbg - *__iter)
											- cart2pol(*_tmp1 - *__iter)) - M_PI)) {
				return false;
			}
		}
	}
	return true;
}

template<typename FWIter> bool iopolygon(const FWIter &_Bbg, const FWIter &_Bed,
		const FWIter &_Ibg, const FWIter &_Ied) {
	bool _flag;
	double _angle, _tmpagl;
	FWIter _tmp1, _tmp2;
	for (FWIter __iter = _Ibg; __iter != _Ied; ++__iter) {
		_flag = false;
		_angle = 0;
		for (_tmp1 = _Bbg, _tmp2 = _tmp1, ++_tmp2; _tmp2 != _Bed;
				++_tmp1, ++_tmp2) {
			if (*__iter == *_tmp1) {
				_flag = true;
				break;
			}
			_tmpagl = mmod(
					cart2pol(*_tmp2 - *__iter) - cart2pol(*_tmp1 - *__iter),
					2 * M_PI);
			if (is_zero(fabs(_tmpagl) - M_PI)) {
				_flag = true;
				break;
			}
			_angle +=
					(mmod(_tmpagl, 2 * M_PI) > M_PI) ?
							_tmpagl - 2 * M_PI : _tmpagl;
		}
		_tmpagl = mmod(cart2pol(*_Bbg - *__iter) - cart2pol(*_tmp1 - *__iter),
				2 * M_PI);
		if (_flag) {
			continue;
		} else {
			if (*__iter != *_tmp1 && !is_zero(fabs(_tmpagl) - M_PI)
					&& is_zero(
							_angle +=
									(mmod(_tmpagl, 2 * M_PI) > M_PI) ?
											_tmpagl - 2 * M_PI : _tmpagl)) {
				return false;
			}
		}
	}
	return true;
}

#endif /* MATLAB_FUN_HPP_ */
