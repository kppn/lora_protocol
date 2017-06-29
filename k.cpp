#include <iostream>
#include <bitset>
#include <typeinfo>

using namespace std;

class Hoge{};

int main()
{
	Hoge hoge;

	cout << (typeid(hoge) == typeid(Hoge)) << endl;

	return 0;
}

