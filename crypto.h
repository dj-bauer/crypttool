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
#include <string>
#include <vector>

using namespace std;

#define MAXN 100001

namespace Crypto
{
void calculate_prime_factors();
extern uint spf[];
namespace Caesar
{
	string encrypt(string clear, int key);
	string decrypt(string ciffer, int key);
	char shift_letter(char letter, int key);
	uint* analyze(string ciffer);
}
namespace Vigenere
{
	string encrypt(string clear, string key);
	string decrypt(string ciffer, string key);
	vector<vector<uint>> analyze(string ciffer, int key_len);
	double friedman_de(string ciffer);
	vector<uint> kasiski(string ciffer, uint max_len, uint min_length = 1);
	vector<uint> kasiski_count(string ciffer, uint max_len, uint min_length = 1);
}
}
