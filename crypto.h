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
