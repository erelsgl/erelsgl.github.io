/*
 * File cpplib/typedefs.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/typedefs.h
 */

/* typedefs.h */
/*
	This unit defines some useful types:
		bool (true, false)
		tristate (true, false, both)
		fourstate (true, false, both, none)
		CStr (constant string -- const char*)
		uint (unsigned int), ulong, ushort, uchar, schar (signed char)
		Index (uint), Size (uint)

	It also defines the useful macro:
					DEFINE_INLINE_RELATIVE_OPERATORS (TheType)
		which uses the compare-function that is defined for TheType, and defines
		all 6 relative operators for TheType.

	And it also defines the 'compare' function
		the 'duplicate' function, the 'hashvalue' function and the 'free' function for all	
		the elementary types (int, char,...)
		'free' for the simple types does nothing.

	It also defines the template function:
				Data duplicate (DataCR the_source)
	which uses the 'void duplicate (DataR to, DataCR from)' to return a duplicate
		of the-source.
*/

#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <string.h>
#include <iostream>
#include <cpplib/status.h>

/***************************** Type bool **************************/

#pragma warning (disable: 4237)     // disable the message: "nonstandard extension used: 'bool' keyword is reserved for future use"

#ifndef bool
//typedef unsigned char bool;
#endif

#define true 1
#define false 0
#define bool_str(b) ( b? "true": "false" )
#define yesno_str(b) ( b? "": "not " )

typedef char tristate;  
#define both 2         
#define tristate_str(b) ( b==2? "both": b==1? "true": "false" )

typedef char fourstate;
#define none 3
#define fourstate_str(b) ( b==3? "none": b==2? "both": b==1? "true": "false" )


/***************************** other useful types *************************/

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef signed char schar;

typedef uint Index;
typedef uint Size;

typedef const char* CStr;

#define DataR Data&
#define DataCR const Data&
#define KeyR Key&
#define KeyCR const Key&

/***************************** relative operator definition macors  *****/

#define DEFINE_INLINE_RELATIVE_OPERATOR(T,op)\
	inline	bool operator op(const T& a, const T& b) \
	 {  return compare(a,b) op 0; }

#define DEFINE_INLINE_RELATIVE_OPERATORS(T)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,>)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,<)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,==)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,<=)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,>=)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,!=)




/**** definition of useful functions for elementary types ********/

#define DEFINE_USEFUL_FUNCTIONS(T)\
	inline short compare (T a, T b) {return  a>b? 1: a==b? 0: -1;}\
	inline bool  identical (T a, T b) {return  a==b;}\
	inline void duplicate (T& to, const T& from) { to=from; }\
	inline void free (T&) { }


DEFINE_USEFUL_FUNCTIONS(schar)
DEFINE_USEFUL_FUNCTIONS(uchar)
DEFINE_USEFUL_FUNCTIONS(char)
DEFINE_USEFUL_FUNCTIONS(short)
DEFINE_USEFUL_FUNCTIONS(int)
DEFINE_USEFUL_FUNCTIONS(uint)
DEFINE_USEFUL_FUNCTIONS(long)
DEFINE_USEFUL_FUNCTIONS(ulong)
DEFINE_USEFUL_FUNCTIONS(float)
DEFINE_USEFUL_FUNCTIONS(double)
	
inline int hashvalue (schar a) { return a; }
inline uint hashvalue (uchar a) { return a; }
inline int hashvalue (short a) { return a; }
inline int hashvalue (int a) { return a; }
inline uint hashvalue (uint a) { return a; }
inline long hashvalue (long a) { return a; }
inline ulong hashvalue (ulong a) { return a; }

inline short compare (CStr a, CStr b) { return strcmp(a,b); }
inline long hashvalue (CStr a) {  
	long sum=0; 
	for (Index i=0; i<strlen(a); ++i)   sum += a[i];
	return sum;
}
inline void duplicate (CStr& to, const CStr& from) { to=from; }
inline void free (CStr&) {  }


template <class Data> Data duplicate (DataCR the_source) {
	Data the_dup;
	duplicate (the_dup, the_source);
	return the_dup;
}


#endif
