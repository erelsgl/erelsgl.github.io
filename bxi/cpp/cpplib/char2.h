/*
 * File cpplib/char2.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/char2.h
 */

/* char2.h -- type char2 -- double character */

#ifndef __CHAR2_H
#define __CHAR2_H

#include <cpplib/typedefs.h>

/********************* type char2 (double char) *****************************************/

class char2 {
	char myinfo[2];
public:
	void operator= (CStr s) { myinfo[0]=s[0];  myinfo[1]=s[1]; }
	char2(CStr s) { myinfo[0]=s[0];  myinfo[1]=s[1]; }
	char2() {myinfo[0]=myinfo[1]='\0';}
	//char operator[] (uint i) const { return myinfo[i]; }
	operator CStr() const { return myinfo; }
};
typedef const char2& char2CR;
inline short compare (char2CR a, char2CR b) {
	return  a[0]>b[0]? 1: a[0]<b[0]? -1: a[1]>b[1]? 1: a[1]<b[1]? -1: 0;
}
DEFINE_INLINE_RELATIVE_OPERATORS(char2)
inline bool operator== (char2CR a, CStr b) { return a[0]==b[0] && a[1]==b[1]; }
inline bool operator== (CStr a, char2CR b) { return a[0]==b[0] && a[1]==b[1]; }

#endif

