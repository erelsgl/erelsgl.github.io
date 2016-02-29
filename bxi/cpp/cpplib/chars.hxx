/* chars.h */
/*
	This unit defines a fixed array of chars.
	The size of the array is a template parameter.
*/

#ifndef __CHARS_H
#define __CHARS_H

#include <cpplib/typedefs.h>
#include <cpplib/stringt.h>

#define TEMPLATE template<Size s>
#define CharsR  Chars<s>&
#define CharsCR const Chars<s>&

TEMPLATE struct Chars {
	char myArray[s];
	Chars(int ZERO=0) { myArray[0]='\0'; }
	CStr str() const { return myArray; }
};

TEMPLATE inline short compare (CharsCR a, CharsCR b) { return compare(a.str(),b.str()); }
TEMPLATE inline long hashvalue (CharsCR a) {  return hashvalue(a.str()); }
TEMPLATE inline void duplicate (CharsR to, CharsCR from) { strcpy(to.myArray, from.str()); }
TEMPLATE inline void free (CharsR) {  }

TEMPLATE inline void write (ostream& out, CharsCR a, Format format="") { out << a.str(); }
TEMPLATE inline status read  (istream& in,  CharsR a, Format format="\n") { 
	in >> ws;
	StringTemp temp(s);
	DOr(read(in, temp ,format));
	strcpy(a.myArray, temp.str);
	return OK;
}


#endif
