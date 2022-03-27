/* Copyright (C) 2022 Daniel Bauer- All Rights Reserved
 *
 * This file is part of DJ-Crypt.
 * DJ-Crypt is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 * DJ-Crypt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Foobar.
 * If not, see <https://www.gnu.org/licenses/>.
 */
#include <vector>
#include <string>

#define uint unsigned int
using namespace std;

namespace Util {
template <typename T>
void print_vector_t(vector<T> data);
void print_vector(vector<string> data);
void print_vector(vector<uint> data);
void print_vector_vector(vector<vector<uint>> data);
}

namespace ImPlotUtil
{
void PlotBarGroups(vector<string> ilabels, vector<vector<uint>>);
}
