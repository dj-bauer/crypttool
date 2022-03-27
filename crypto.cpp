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
#include "crypto.h"

#include "util.h"

#define uint unsigned int

#include <iostream>
#include <algorithm>

namespace Crypto
{
	uint spf[MAXN];

	static bool is_alpha(char letter)
	{
		return !(('Z' < letter || 'A' > letter) && ('z' < letter || 'a' > letter));
	}
	static string to_upper(string input)
	{
		for(char& c : input)
			c = toupper(c);
		return input;
	}
	void calculate_prime_factors()
	{
		spf[1] = 1;
		// Erstmal sich selbst als kleinsten Primfaktor setzen
		for(uint i=2; i<MAXN; i++)
			spf[i] = i;
		// Für alle geraden zahlen 2 setzen
		for(int i=4; i<MAXN; i+=2)
			spf[i] = 2;
		for(uint i=3; i*i<MAXN; i++)
		{
			// Wenn i eine Primzahl ist.
			// Wäre i keine Primzahl war der Wert schon vorher auf etwas anderes als i gesetzt worden
			if(spf[i] == i)
			{
				// Alle zahlen, die durch i teilbar sind auf i als kleinster Faktor sezten, wenn kein kleinerer vorher gesetzt wurde
				for(uint j=i*i; j<MAXN; j+=i){
					if(spf[j] == j)
						spf[j] = i;
				}
			}
		}
	}
	static vector<uint> get_prime_factors(uint number)
	{
		vector<uint> result;
		while(number != 1)
		{
			result.push_back(spf[number]);
			number = number / spf[number];
		}
		return result;
	}

namespace Caesar
{
	string encrypt(string clear, int key)
	{
		string ciffer;
		for(const char& c : clear)
			ciffer += shift_letter(c, key);
		return ciffer;
	}
	string decrypt(string ciffer, int key)
	{
		string clear;
		for(const char& c : ciffer)
			clear += shift_letter(c, (26-key));
		return clear;
	}
	char shift_letter(char letter, int key)
	{
		if(!is_alpha(letter))
			return letter;
		uint index = letter > 'Z' ? letter - 'a' : letter - 'A';
		index = (index + key) % 26;
		char result = letter > 'Z' ? 'a' + index : 'A' + index;
		return result;
	}
	uint* analyze(string ciffer) {
		uint* result = (uint*) malloc(sizeof(uint) * 26);
		for(int i=0; i<26; i++) result[i] = 0;

		for(char& c : ciffer) {
			if(!is_alpha(c))
				continue;
			c = toupper(c);
			int index = c - 'A';
			result[index]++;
		}

		return result;
	}
}
namespace Vigenere
{
	string encrypt(string clear, string key)
	{
		key = to_upper(key);
		string ciffer = "";
		uint key_index = 0;
		for(char& c : clear) {
			if(key_index == key.length()) key_index = 0;
			if(!is_alpha(c))
				ciffer += c;
			else {
				int index = key[key_index] - 'A';
				ciffer += Crypto::Caesar::shift_letter(c, index);
				key_index++;
			}
		}

		return ciffer;
	}
	string decrypt(string ciffer, string key)
	{
		key = to_upper(key);
		string clear = "";
		uint key_index = 0;
		for(char& c : ciffer) {
			if(key_index == key.length()) key_index = 0;
			if(!is_alpha(c))
				clear += c;
			else {
				int index = key[key_index] - 'A';
				clear += Crypto::Caesar::shift_letter(c, (26-index));
				key_index++;
			}
		}
		return clear;
	}
	vector<vector<uint>> analyze(string ciffer, int key_len)
	{
		vector<vector<uint>> result;
		for(int i=0; i<key_len; i++) {
			vector<uint> list(26);
			for(int j=0; j<26; j++)
				list[j] = 0;
			result.push_back(list);
		}

		ciffer = to_upper(ciffer);

		int text_len = ciffer.length();
		int key_mult = 0;
		int offset = 0;
		while(key_mult * key_len < text_len) {
			for(int key_index = 0; key_index < key_len; key_index++) {
				int text_index = key_mult * key_len + key_index + offset;
				if(text_index >= text_len) return result;

				char ch = toupper(ciffer[text_index]);
				if(!is_alpha(ch)) {
					offset++;
					key_index--;
				} else {
					result[key_index][ch - 'A']++;
				}
			}
			key_mult ++;
		}
		return result;
		
	}
	double friedman_de(string ciffer)
	{
		string alpha_text;
		for(const char& ch : ciffer) {
			char c = toupper(ch);
			if(is_alpha(c)) alpha_text += c;
		}
		uint* v = Caesar::analyze(alpha_text);
		double n = alpha_text.length();
		double I = 0;
		for(int i=0; i<26; i++) {
			I += (double) v[i] * ((double) v[i] - 1.0);
		}
		I /= (n*(n-1.0));
	    double h = (0.0377*n) / (I * (n-1) - 0.0385*n + 0.0762);
		return h;
	}
	/*
	 * TODO: This method is very unoptimized and there should be many ways to improve its performance
	 */
	vector<uint> kasiski(string ciffer, uint max_len, uint min_length)
	{
		string text;
		for(const char& ch : ciffer) {
			char c = toupper(ch);
			if(is_alpha(c)) text += c;
		}
		// Find the patterns
		vector<uint> distances;
		uint text_len = text.length();
		for(uint len = max_len; len > min_length; len--) {
			uint index = 0;
			while(index + len < text_len) {
				string to_compare = text.substr(index, len);
				for(uint running=index+len; running < text_len; running++) {
					string checking = text.substr(running, len);
					if(checking.compare(to_compare) == 0) {
						//cout << "I found a match of: " << to_compare << " at: " << index << " and " << running << "\n";
						distances.push_back(running - index);
						break;
					}
				}
				index++;
			}
		}

		vector<uint> result;
		for(uint d : distances) {
			vector<uint> factors = get_prime_factors(d);

			// Sort and unique and then only append the unique items
			sort(factors.begin(), factors.end());
			auto last = unique(factors.begin(), factors.end());
			result.insert(result.end(), factors.begin(), last);
		}
		return result;
	}
	vector<uint> kasiski_count(string ciffer, uint max_len, uint min_length)
	{
		vector<uint> result;
		vector<uint> data = kasiski(ciffer, max_len, min_length);
		for(uint v : data)
		{
			uint len = result.size();
			if(v >= len) {
				for(uint i=0; i<=v-len; i++) {
					result.push_back(0);
				}
			}
			result.at(v)++;
		}
		return result;
	}
}
}
