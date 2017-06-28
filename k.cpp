#include <iostream>
#include <bitset>

using namespace std;

int main()
{
	char a[1] = {(char)0x80};
	//string s(reinterpret_cast<char *>a, 1);
	string s(a, 1);
	cout << s << endl;

	return 0;
}

