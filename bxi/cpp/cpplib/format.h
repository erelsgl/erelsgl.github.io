/*
 * File cpplib/format.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/format.h
 */

/* format.h -- the Format class */

#ifndef __FORMAT_H
#define __FORMAT_H

#include <cpplib/typedefs.h>
#include <cpplib/status.h>
#include <string.h>
#include <ctype.h>
#include <iostream.h>

/***********************************************************************************
	The Format class is used as a parameter to output routines, to set various
		format parameters, such as separators in list-output.
	The Format class really encapsulates a constant string. (const char*).
	A typical output routine that uses a Format parameter looks like this:
		void write (ostream& out, const List& thelist, Format theformat) {...}
	The meaning of this routine can be:
		"write out the list 'thelist' to the stream 'out', using as a separator
		the first character of the string in 'theformat'."
		For example: 
			write (cout,thelist,Format(","))
		means to write thelist to the screen (cout), using comma as a separator.
	Of course, the 'write' routine can use the format string in many other ways.

    The Format class can also be used in input routines. For example, it can be used
		as a 'character set' that determines the input delimiters.
	
***********************************************************************************/

inline bool contains (CStr str, char c) { return (strchr(str,c)!=0); }

class Format  {
		CStr mystr;
	public:
		Format (CStr thestr="") { mystr=thestr; }
		Format (char thechar)    { char* c=new char[2]; c[0]=thechar; c[1]='\0'; mystr=c; }
		Format (char thestr[])  { mystr=thestr; }
		Format (const Format& theformat) { mystr=theformat.str(); }

		CStr str() const { return mystr; }
		bool isempty() const { return mystr==NULL || mystr[0]=='\0'; }
		char operator[] (Index i) const { return mystr[i]; }

		Format next () const {
			if ( this->isempty() ) return *this;
			else return Format (mystr+1);
		}

		void operator++() {
			if (!this->isempty())  mystr++;
		}

		void separate(ostream& out) const {
			// uses the first char of mystr as an output separator
			if (!this->isempty())
				out << mystr[0];
		}

		status testchar (istream& in) const {
			// tests to see that the next char in 'in' is equal to the FIRST char of mystr.
			if (this->isempty()) return OK;
			if (mystr[0]==' ') { in.get(); return OK; }
			else if (in.peek()==mystr[0]) {	in.get();return OK; }
			else return EBADFORMAT;
		}

		bool delimits (istream& in) {    
			// returns true if the next char of in is ONE OF the chars of mystr.
			char c = in.peek();
			if (mystr[0]=='^')        // This is the 'complement' char 
				return (!contains(mystr+1,c));
			else 
				return (contains(mystr,c));
		}	
};


#endif

