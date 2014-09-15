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
 * MAFM.hpp
 *
 *  Created on: May 22, 2013
 *      Author: guangmu
 */

#ifndef MAFM_HPP_
#define MAFM_HPP_

#include <string>
#include <vector>
#include <utility>

#include "matlab_fun.hpp"
#include "VET.hpp"

bool load_off(const std::string&, std::vector<std::vector<Point>>&);

bool check_off(std::vector<std::vector<Point>>&);

bool _is_inner_outof_outer(const std::vector<std::vector<Point>>&);

bool _has_intersecting_lines(const std::vector<std::vector<Point>>&);

void _check_direction(std::vector<std::vector<Point>>&);

std::vector<matrix> boundary_interpolation(
		const std::vector<std::vector<Point>>&, double);

matrix inner_interpolation(const std::vector<std::vector<Point>>&,
		const matrix&, double);

matrix filter_inner_interpolation(const std::vector<matrix>&, const matrix&,
		double);

// create triangles
std::pair<matrix, matrix> create_triangles(const std::vector<matrix>&,
		const matrix&, double);

// smooth triangles
std::pair<matrix, double> smooth_triangles(matrix, unsigned,
		const matrix&, double, double, double, double);

void save_OFF(const matrix&, const matrix&, const std::string&);

#endif /* MAFM_HPP_ */
