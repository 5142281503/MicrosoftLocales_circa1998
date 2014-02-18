#include <locale>
#include <cassert>

using namespace std;

int main(){

	assert(LC_COLLATE == collate<char>::_Getcat());
	assert(LC_CTYPE == ctype<char>::_Getcat());
	assert(LC_MONETARY == money_put<char>::_Getcat());
	assert(LC_MONETARY == money_get<char>::_Getcat());
	assert(LC_NUMERIC == numpunct<char>::_Getcat());
	assert(LC_TIME == time_put<char>::_Getcat());
	assert(LC_TIME == time_get<char>::_Getcat());

	return 1;
}