#include "util.h"
#include "implot.h"

#include <iostream>

namespace Util
{
	template <typename T>
	void print_vector_t(vector<T> data)
	{
		cout << "[";
		bool first = true;
		for(T el : data) {
			if(!first) {
				cout << ", ";
			}
			first = false;
			cout << el;
		}
		cout << "]\n";
	}
	void print_vector(vector<string> data) {
		print_vector_t(data);
	}
	void print_vector(vector<uint> data) {
		print_vector_t(data);
	}

	void print_vector_vector(vector<vector<uint>> data) {
		cout << "[\n";
		for(vector<uint> v : data)
			print_vector_t(v);
		cout << "]\n";
	}
}

namespace ImPlotUtil
{
void PlotBarGroups(vector<string> ilabels, vector<vector<uint>> data)
{
	uint num_groups = data.size();
	uint num_elements = data[0].size();

	vector<const char*> ilabels_c;
	for(string l : ilabels) {
		char* c = new char[l.length() + 1];
		strcpy(c, l.c_str());
		ilabels_c.push_back(c);
	}
	uint* data_c = new uint[num_groups * num_elements];
	for(uint i=0; i<num_groups; i++) {
		for(uint j=0; j<data[i].size(); j++) {
			data_c[i*num_elements + j] = data[i][j];
		}
	}

	ImPlot::PlotBarGroups(ilabels_c.data(), data_c, num_groups, num_elements);
}
}
