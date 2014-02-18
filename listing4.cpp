/* ctype helper functions */

#include <iostream>
#include <locale>

using namespace std;

int main(){

	const char a = 'a';

	if(isalpha<char>(a))
		cout << a << " is an alphabetic character" << endl;
	else
		cout << a << " is not an alphabetic character" << endl;

	char b = 'b';
	if(islower<char>(b))
		cout << b << " is a lowercase character" << endl;
	else
		cout << b << " is not a lowercase character" << endl;

	b = toupper<char>(b);	
	if(islower<char>(b))
		cout << b << " is a lowercase character" << endl;
	else
		cout << b << " is not a lowercase character" << endl;

	return 1;
}
