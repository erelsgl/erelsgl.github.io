/*
 * File cpplib/argsfile.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/argsfile.cpp
 */

/* argsfile.cpp -- a unit to read a program's arguments file */

#include "argsfile.h"

#include <cpplib/stringt.h>
#include <cpplib/trie0.hxx>
#include <cpplib/chars.hxx>

typedef Chars<30> argumentString;
DEFINE_INLINE_IO_OPERATORS(argumentString);
DEFINE_INLINE_RELATIVE_OPERATORS(argumentString);

Trie0<argumentString> stringArguments;
Trie0<int> intArguments;

void read_arguments_file (CStr thePath)
{
	ifstream in;
	DOx(open (thePath, in));
	skip_comments(in, '%');
	DOx(read (in, stringArguments, "T: "));
	writeln(cerr, stringArguments);
	skip_comments(in, '%');
	DOx(read (in, intArguments, "T: "));
	writeln(cerr, intArguments);
}


CStr string_argument (CStr theName) {
	argumentString temp;
	assert (stringArguments.contains(theName, temp));
	return StringTemp(temp.str()).finalstr();
}

int int_argument (CStr theName) {
	int temp;
	assert (intArguments.contains(theName, temp));
	return temp;
}


CStr string_argument (CStr theName, CStr theDefault) {
	argumentString temp;
	if (stringArguments.contains(theName, temp)) 
		return StringTemp(temp.str()).finalstr();
	else
		return theDefault;
}

int int_argument (CStr theName, int theDefault) {
	int temp;
	if (intArguments.contains(theName, temp)) 
		return temp;
	else
		return theDefault;
}
