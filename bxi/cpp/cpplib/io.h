/*
 * File cpplib/io.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/io.h
 */

/* io.h -- input/output handling routines */
/* 
	This unit defines the input/output routines 'write' and 'read' for all elementary types.
	It also defines the macro 
							DEFINE_INLINE_IO_OPERATORS (TheType)
		which uses the write/read routines of TheType to create the I/O operators (<<,>>)
		for TheType.
	
	It also defines the template routine:
		void writeln (ostream& the_output, Data the_data, Format the_format)
	which does like 'write', but also prints 'return' afterwards.
	
	It also defines the following functions, which are useful in input routines:
		void skip (the_input, the_char_set);
		void skipto (the_input, the_char_set);
		void skip_comments (the_input, the_comment_char);
		status testchar (the_input, the_char);
	
*/

#ifndef __IO_H
#define __IO_H

#include <cpplib/format.h>


/*********************** define useful IO functions for the simple types ************/

#define DEFINE_USEFUL_IO_FUNCTIONS(T)\
	inline void write (ostream& out, T a, Format format="") { out<<a; }\
	inline status read (istream& in, T& a, Format format="") \
		{ in>>a; return (in.good()? OK: EBADFORMAT); }\
	inline void save (ostream& out, T a) { out<<a; }\
	inline void load (istream& in, T& a) { in>>a; }

#define DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(T)\
	inline void write (ostream& out, T a, Format format="") { out<<a; }\
	inline status read (istream& in, T& a, Format format="") \
		{	in >> ws; \
			bool negative=false;   \
			char c=in.get(); \
			if ( c=='-' && isdigit(in.peek()) )   { c=in.get(); negative=true; } \
			if (!isdigit(c)) { in.putback(c); return EBADFORMAT; } \
			for (a=0;;) { \
				a = a*10 + (c-'0'); \
				c = in.get(); \
				if (!isdigit(c) ) { in.putback(c); return OK; } \
			} \
		}\
	inline void save (ostream& out, T a) { out<<a; }\
	inline void load (istream& in, T& a) { in>>a; }


DEFINE_USEFUL_IO_FUNCTIONS(char)
DEFINE_USEFUL_IO_FUNCTIONS(schar)
DEFINE_USEFUL_IO_FUNCTIONS(uchar)
//DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(byte)
//DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(ubyte)
DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(short)
DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(int)
DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(uint)
DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(long)
DEFINE_USEFUL_IO_FUNCTIONS_FOR_INTEGERS(ulong)
DEFINE_USEFUL_IO_FUNCTIONS(float)
DEFINE_USEFUL_IO_FUNCTIONS(double)

inline void write (ostream& o, CStr a, Format format="")   { o<<a; }



/***************************** IO operator definition macors  ********/

#define DEFINE_INLINE_IO_OPERATORS(T)\
inline ostream& operator<< (ostream& out,const T& a)\
 { write (out,a); return out; }\
inline istream& operator>> (istream& in,T& a)\
 { read (in,a); return in; }



/************************ writeln -- writes x and then newline *********************/

template <class T> void writeln (ostream& out, const T& x, Format format)
{
	write (out,x,format);
	out << endl;
}

template <class T> void writeln (ostream& out, const T& x) 
{
	write (out,x);
	out << endl;
}	



//  these routines are implemented in KLLI1.CPP
void skip (istream& in, CStr thechars); // example: skip(cin," \t\n") skips whitespace in the standard input.
void skipto (istream& in, CStr thechars); // example: skipto(cin,"\n") skips everything until the end of the line.
void seek (istream& in, CStr thechars); // example: seek(cin,"0123456789") seeks the next digit.
void skip_comments (istream& in, char comment_char);  // example: skip_comments(in,'%') will skip all comments beginning with '%' (to the EOL).
status testchar (istream& in, char c);  // example: testchar (in,':') returns OK iff the next char is ':' (in this case it also extracts the ':' from the input).


#endif

