How Microsoft implemented Locales and Facets in their C++ compiler.
Mario Contestabile

If you've come across the "locale" notion while reading on C++
you may feel somewhat unfulfilled. If you planned on using Bjarne 
Stroustrup's "C++ Programming Language" book to clear your lantern, he brushes
off the topic, stating the discussion "is beyond the scope of this book".
"C++ Primer" by Stanley Lippman and Josee Lajoie will not be of much help 
in this topic either; since their implementation did not support locales
they used the Standard C implementation. I do still highly recommend both
books to anyone interested in serious C++ development.

To begin with, my development environment is MSVC++ 5.0 sp3 and 6.0. The 
code listings provided have been compiled and tested with both compilers.
Note that I use the latest STL (or rather C++ Standard) headers as provided
by P.J. Plauger. Mr. Plauger is president of Dinkumware and he wrote many of 
Microsoft's C++ Standard headers, of which he posts bug fixes at 
http://www.dinkumware.com/vc_fixes.html These fixes have not yet been 
incorporated into MSVC++ 6, which is why I still rely on the P.J's headers.

To understand locales, we will begin with the C programming language. C programs
use locales via the ANSI locale.h header. A locale defines how time,
dates, characters, strings and other expressions are formatted for the current 
country, or cultural area.  In C, the one mandatory, or predefined locale is 
the "C" locale (for United States defaults), and the empty string, "", meaning 
implementation defined. The "C" locale assumes that all char data types
are 1 byte and that their value is usually less than 256. By default, all 
locale-dependent routines in the Microsoft run-time library use the code page
that corresponds to the "C" locale. In C, you retrieve some locale-specific information with the
localeconv() function and set the locale with the setlocale() function. As 
we'll see later, The runtime library always uses the C locale until told otherwise with 
setlocale(). C++ adds functionality as well by wrapping locale information in
classes. You can then have more than one of these locale objects alive at the same
time.

Listing 1 shows the simplest example of a C application using locales by 
displaying the date in the "C" and the "french" locales. You'll notice the
program specifies LC_ALL in setlocale(), which is a #define in locale.h.
We can change a specific category of a locale by specifying it as the parameter
to setlocale(). In our example, we could of used LC_TIME, and the results would
of been identical. On the other hand, specifying LC_NUMERIC would not of 
affected the date displayed. The locale categories are

LC_COLLATE		Which is used by strcoll() and strxfrm()
LC_CTYPE		Which is used by the character handling functions in <ctype.h>
LC_MONETARY		Monetary formatting, no C functions use this information except localconv()
LC_NUMERIC		For non-monetary formatting.
LC_TIME			Which is used by strftime(), not ctime() or asctime()

Retrieving locale information with the localeconv() function returns a struct lconv
object. A quick glance into the <locale.h> header reveals the structures innards.
Listing 2 shows the default values for each lconv member in the "C" locale. To demonstrate
some of these default values, Listing 3 retrieves and displays some "C" locale default
information using the localeconv() function.

By putting our C++ thinking cap on, we can quickly get a feel of how all this could
be done in C++; wrap each locale category in a separate class, have a generic locale
class container default to the "C" behavior for each of the locale classes. Then if
someone needs any of those locale objects to behave in a specific way, he can derive 
his own class from them and override the necessary virtual functions.

Given our C locale categories, let's begin by determining which classes handle each
category. Following is the category, C++ Standard class name and the header file 
where the class is located. You only need to include <locale> to use any of these:

Locale category		C++ class name		Header

LC_COLLATE			collate				<Locale>
LC_CTYPE			ctype				<Xlocale>
LC_MONETARY			money_put/money_get	<Xlocmon>
LC_NUMERIC			numpunct			<Xlocnum>
LC_TIME				time_get/time_put	<Xloctime>


All these classes are templates, capable of handling
the 'char' and 'wchar_t' types. This can be a real help for developers creating ANSI
and UNICODE applications. The standard library doesn't support MBCS, only single-byte
and wide characters. All of these classes have a _Getcat() member function
which returns the LC_xxx category. Listing 4 shows the locale category associated
with each class.

The C++ framework for the aforementioned classes is as follows; All these classes are
derived from the locale::facet base class. _Getcat() for example is a member
of locale::facet which simply returns -1. The reason for the locale::facet
syntax is because facet is a local class of the enclosing locale class.
So, the facet class actually contains information on the localization aspect
whereas the locale class is a container of facets. To use a facet, you use the 
Standard provided global function template use_facet(). More on use_facet() later.

You may be thinking _Getcat() is one of many virtual functions which behaves 
polymorphically in the derived classes, and has defaults in the base class (returning -1
in this case). In fact, neither locale nor facet have any virtual functions (with
the exception of the facet destructor). So then, how do we extend the behavior of
these facets? You do so by adding your own unique facet to a locale, with
locale::_Addfac(), specifying the facet and the facet Id. All of this machinery can
be found in \crt\src\locale.cpp. A locale cannot contain two facets with the same id.
So when we'll derive new classes from those previously mentioned, they'll have the
same id as the original class, and will therefore replace them. For example,
if Mynumpunct is derived from numpunct, and we then call _ADDFAC(locale(), Mynumpunct),
the facet Id will be the same as numpunct::Id. Listing 5 shows how a class derived
from numpunct has the same Id as numpunct itself. Once created a locale and its
facet cannot change. _Getcat() and _Addfac() are nonstandard MSVC additions.


A special note about the ctype class; for efficiency, the C++ Standard requires
that ctype for chars be implemented as a template specialization. For this reason
ctype differs somewhat from the other classes in that it derives from ctype_base,
which itself is derived from locale::facet, as opposed to being directly derived from
locale::facet.

ctype<char> : public ctype_base
template<class _E> class ctype : public ctype_base

The ctype_base contains an enumeration indicating a character's particular semantics.
This allows very fast character classification using bit masks. Listing 6 shows how
ctype can be used for determining character semantics. I recommend stepping through
all the examples listed to better grasp all concepts presented. You might be surprised to
find the implementation for class locale in locale.cpp and local0.cpp, in the "\crt\src" 
directory. I would of thought a "\cpp\src" directory structure would of been more appropriate.

Note the _DIGIT #define which Listing 6 uses. This checks if the character
is in the '0'-'9' range. Other available bit masks are:

_UPPER to determine if it is an upper case letter A-Z
_LOWER to determine if it is a lower case letter a-z
_SPACE to determine if it is a horizontal tab, carriage return, newline, vertical tab or form feed
_PUNCT to determine if it is a punctuation character
_CONTROL to determine if it is a control character such as BEL and backspace
_BLANK to determine if it is a space
_HEX to determine if it is a hexadecimal digit (0-9, A-F or a-f)

You'll find the definitions for these in <wchar.h> and in <Xlocinfo.h>. If the syntax
seems contrived to you, <Locale> provides some support templates. Using those helper 
templates, Listing 6 can be rewritten in a clearer fashion as shown in Listing 7.

Previously I mentioned that to access a facet contained in a local you call
the use_facet function. As you may of noticed in the sample listings, I use 
a macro called _USE(). _USE() is simply defined as
#define _USE(loc, fac)         use_facet(loc, (fac *)0, true)
in this implementation. The first parameter is a locale, and the second parameter 
is a facet. I often use locale::classic() or locale::empty() as the locale parameter.
empty() is a nonstandard locale object with no facets which behaves differently from locale().
Whereas classic() is the locale that C defines, empty() makes a transparent locale where
any missing facets permit facets from the global locale to shine through.
use_facet is a template function which returns
a reference to the locale facet found in the specified locale. It does this by calling
locale::_Getfacet(). If there is no facet in the locale object, it simply throws bad_cast. 
We could ask use_facet to create the facet for us, that is what the third parameter
is for. Passing true, as is always the case with the _USE() macro,
asks use_facet to new the facet. Another macro, _USEFAC, is exactly like
_USE() except it passes false to use_facet, asking that the facet not be new'ed.
The use of true as a third argument is designed to support lazy evaluation
of the standard facets and should not be used indiscriminately.
Listing 8 shows what happens when attempting to use a facet without passing true
to use_facet. The facet returned by these two macros is locale::_Locimp::facet::_Fv. 
P.J. had to provide these two macros because at the time the MSVC++ compiler did not 
support explicit template argument specification. Listing 9 shows what happens when a
function foo() has a template argument which the compiler cannot determine on its own.
The function bar() shows a similar problem, in the call to bar(1., 2), the two parameters
are of different types, so the compiler does not know which of the two types Type should be,
so it chooses not to decide by flagging it as an error.

With all of this framework background we're ready to implement our own facets
and use them within a real-world C++ Windows application. Win32 provides many functions similar
to the facet functionality. IsCharAlpha() for example is similar to isalpha<char>() except that 
it uses the information specified by the user in the control panel. For Windows applications,
this is preferable, except for the fact that it would leave out all the Standard locale
and facet classes we've come to love. What we need to do is implement our own facets
which make use of the Win32 APIs. That would allow us to program in a consistent fashion.

We can use GetLocaleInfo() to retrieve information about the current user locale
and implement our own facet which makes use of this information. Listing 10 shows
our Dmoneypunct facet in action. In between executions, if you go in Control Panel and
select the Regional Settings Icon and go in the Currency tab, you can make changes and
they will immediately be used by the Dmoneypunct class.

Special thanks to P.J. Plauger for his technical proof reading and comments.
