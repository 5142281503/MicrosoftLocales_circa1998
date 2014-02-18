/* C++ and Win32 hand-in-hand */

#include <windows.h>
#include <locale>
#include <iostream>

class Dmoneypunct : public std::moneypunct<TCHAR, true>{
	mutable TCHAR buf[20];
protected:
	virtual TCHAR do_decimal_point() const {
		if(0 != GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, buf, sizeof buf))
			return buf[0];
		else
			return std::_USE(std::locale(), std::moneypunct<TCHAR>).decimal_point();		
	}
	virtual TCHAR do_thousands_sep() const {		
		if(0 != GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, buf, sizeof buf))
			return buf[0];
		else
			return std::_USE(std::locale(), std::moneypunct<TCHAR>).thousands_sep();
	}
	virtual std::string do_grouping() const {		
		if(0 != GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONGROUPING, buf, sizeof buf))
			return buf;
		else
			return std::_USE(std::locale(), std::moneypunct<TCHAR>).grouping();
	}
	virtual std::string do_curr_symbol() const {		
		if(0 != GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, buf, sizeof buf))
			return buf;
		else
			return std::_USE(std::locale(), std::moneypunct<TCHAR>).curr_symbol();
	}
	virtual std::string do_positive_sign() const {		
		if(0 != GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SPOSITIVESIGN, buf, sizeof buf))
			return buf;
		else
			return std::_USE(std::locale(), std::moneypunct<TCHAR>).positive_sign();
	}
	virtual std::string do_negative_sign() const {		
		if(0 != GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN, buf, sizeof buf))
			return buf;
		else
			return std::_USE(std::locale(), std::moneypunct<TCHAR>).negative_sign();
	}
};

int main(){

	std::moneypunct<TCHAR, true> i;
	std::cout<< "moneypunct decimal point " << i.decimal_point() << std::endl;
	std::cout<< "moneypunct thousands sep " << i.thousands_sep() << std::endl;
	std::cout<< "moneypunct grouping " << i.grouping() << std::endl;
	std::cout<< "moneypunct currency symbol " << i.curr_symbol() << std::endl;
	std::cout<< "moneypunct positive sign " << i.positive_sign() << std::endl;
	std::cout<< "moneypunct negative sign " << i.negative_sign() << std::endl;

	std::cout<< std::endl;

	Dmoneypunct D;
	std::cout<< "Dmoneypunct decimal point " << D.decimal_point() << std::endl;
	std::cout<< "Dmoneypunct thousands sep " << D.thousands_sep() << std::endl;
	std::cout<< "Dmoneypunct grouping " << D.grouping() << std::endl;
	std::cout<< "Dmoneypunct currency symbol " << D.curr_symbol() << std::endl;
	std::cout<< "Dmoneypunct positive sign " << D.positive_sign() << std::endl;
	std::cout<< "Dmoneypunct negative sign " << D.negative_sign() << std::endl;
	
	return 1;
}