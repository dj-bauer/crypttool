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
