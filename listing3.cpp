/*  ctype<char> is a specialized template for efficiency */

#include <iostream>
#include <locale>

using namespace std;

int main(){

	const char a = 'a';
	
	if(_USE(locale::empty(), ctype<char>).is(_DIGIT, a))
		cout << a << " is a digit" << endl;
	else
		cout << a << " is not a digit" << endl;

	const char b = '1';
	if(_USE(locale::empty(), ctype<char>).is(_DIGIT, b))
		cout << b << " is a digit" << endl;
	else
		cout << b << " is not a digit" << endl;

	return 1;
}