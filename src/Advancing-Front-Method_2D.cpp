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


//============================================================================
// Name        : Advancing-Front-Method_2D.cpp
// Author      : Guangmu Zhu
// email       : guangmuzhu@gmail.com
// Version     : 0.1.0
// Copyright   : Copyright 2013
// Description : Advancing Front Method for Multi-boundary with 2D
//============================================================================

#include <cstdlib>
#include <iostream>
#include <cstdio>

#include "MAFM.hpp"

using namespace std;

int main(int argc, char** argv) {
	try {
		string path;
		cout << "Input File Path: " << flush;
		if (cin >> path) {
			vector<vector<Point>> PVs;
			if (load_off(path, PVs)) {
				if (!check_off(PVs)) {
					cerr << "Bad boundaries. " << endl;
					return EXIT_FAILURE;
				}
				matrix BMXY(2, 2, { PVs.at(0).at(0).x, PVs.at(0).at(0).x,
						PVs.at(0).at(0).y, PVs.at(0).at(0).y });
				for (auto iter = ++PVs.cbegin()->cbegin();
						iter != PVs.cbegin()->cend(); ++iter) {
					if (iter->x < BMXY(1, 1))
						BMXY(1, 1) = iter->x;
					if (iter->x > BMXY(1, 2))
						BMXY(1, 2) = iter->x;
					if (iter->y < BMXY(2, 1))
						BMXY(2, 1) = iter->y;
					if (iter->y > BMXY(2, 2))
						BMXY(2, 2) = iter->y;
				}
				double VStep;
				cout << "VStep: " << flush;
				while (true) {
					cin >> VStep;
					if (cin) {
						if ((BMXY(2, 2) - BMXY(2, 1)) / VStep < 4.5) {
							cerr << "Size is too large." << endl;
							continue;
						} else
							break;
					} else {
						cin.clear();
					}
				}
				vector<matrix> BVs = boundary_interpolation(PVs, VStep);
				matrix IVs = inner_interpolation(PVs, BMXY, VStep);
				IVs = filter_inner_interpolation(BVs, IVs, VStep);
				pair<matrix, matrix> result = create_triangles(BVs, IVs, VStep);
				double rf = 1, CC = 1e-4, EC = 1e-2;
				cin.ignore(1024, '\n');
				cin.clear();
				cout << "Relaxation factor:(default 1) " << flush;
				if (cin.peek() != '\n') {
					cin >> rf;
				}
				cin.ignore(1024, '\n');
				cin.clear();
				cout << "Convergence coefficient:(default 1e-4) " << flush;
				if (cin.peek() != '\n') {
					cin >> CC;
				}
				cin.ignore(1024, '\n');
				cin.clear();
				cout << "Error coefficient:(default 1e-2) " << flush;
				if (cin.peek() != '\n') {
					cin >> EC;
				}
				pair<matrix, double> smooth_result = smooth_triangles(
						result.first, IVs.get_size().first, result.second, rf,
						VStep, CC, EC);
				printf("%.2f%% Error\n", smooth_result.second * 100);
				cout << "Output File Path: " << flush;
				cin.clear();
				if (cin >> path) {
					smooth_result.first = smooth_result.first | 3 | matrix(result.first.get_size().first, 1);
					result.second =
							matrix(result.second.get_size().first, 1).init(3)
									| result.second;
					save_OFF(smooth_result.first, result.second, path);
				} else {
					cout << result.first.to_string("", "", " ") << '\n';
					cout << result.second.to_string("", "", " ") << '\n';
					cout << flush;
				}
				return EXIT_SUCCESS;
			} else
				return EXIT_FAILURE;
		} else
			return EXIT_FAILURE;
	} catch (exception &e) {
		cerr << "Exception occurred: " << e.what() << endl;
		return EXIT_FAILURE;
	}
}
