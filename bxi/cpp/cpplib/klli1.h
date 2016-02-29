/*
 * File cpplib/klli1.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/klli1.h
 */

/* klli1.h -- general routines */
#ifndef __KLLI1_H
#define __KLLI1_H

/*
	This unit defines several useful functions:
		CStr str (the_char) -- converts the-char to a string.
		Index strindex (small,big) -- finds the small string in the big string.
		Index charindex (the_char, the_string) -- finds the char in the string.
		bool strinstring (small,big) -- does 'small' occur in 'big'?
		bool charinstring (the_char,the_string) -- does the-char occur in the-string?
		bool identical (str1,str2);    bool different(str1,str2)  --  string-comparison
		void exit_with_error (message,argument,synopsis) -- exits and prints error message
		status open (filename,input);  status open (filename,output) -- file-open, with error handling.
		char case_convert (source, source_zero, target_zero);
		char parenthesis_reverse (sourcc);
*/




/******** Include all basic header files ******/
#include <fstream>
#include <fstream.h>
#include <iosfwd>
#include <cpplib/status.h>
#include <cpplib/typedefs.h>
#include <cpplib/io.h>
#include <cpplib/stdtempl.h>
#include <cpplib/mem.h>

#ifndef EOL
#define EOL '\n'
#endif

#ifndef EOS
#define EOS '\0'
#endif

#ifndef ESC
#define ESC '\27'
#endif

CStr str(char c);  // converts a char to a constant string
inline CStr str(CStr s) { return s; }    // works over a bug in the compiler: in template functions, it can't accept "abc" as a CStr parameter (so it should be given as str("abc")).

// If name!=NULL, these open routines open the file as usual. If name==NULL, the file will
//	be synchronized to the standard input/output.

typedef short open_mode;
status open (CStr name, ifstream& file, open_mode or_mode = ios::in);
status open (CStr name, ofstream& file, open_mode or_mode = ios::app);


inline Index strindex (CStr small, CStr big) { 
	// it 'small' occurs in 'big' -- return the first index where it occurs. 
	// otherwise return the length of 'big'.
	CStr result=strstr(big,small);
	return (result==NULL? strlen(big): result-big);
}
inline Index charindex (char thechar, CStr thestring) { 
	CStr result=strchr(thestring,thechar);
	return (result==NULL? strlen(thestring): result-thestring);
}
inline bool strinstring (CStr thestr, CStr thestring) { return strstr(thestring,thestr)!=NULL; }
inline bool charinstring (char thechar, CStr thestring) { return strchr(thestring,thechar)!=NULL; }

inline bool identical (CStr a, CStr b) { return strcmp(a,b)==0; }
inline bool different (CStr a, CStr b) { return strcmp(a,b)!=0; }

void write_str (ostream& out, CStr the_str, int from, int to);  // write the_str[from..to-1]. if 'from' or 'to' are negative, they meen 'from the end of the string'.

void exit_with_error (const char* error_type, const char* argument, const char* synopsis=NULL);
long filesize (char* path);



char case_convert (uchar source, uchar source_zero, uchar target_zero);
/*
  This function is used to convert a char between character set.
  For example, if you want to convert from 7-bit Hebrew (where Aleph==96)
  to 8-bit Hebrew (where Aleph==128), use:
     case_convert (letter, 96, 128)
*/

char parenthesis_reverse (char source);
/* if 'source' is a parenthesis -- reverses it (e.g. [ ==> ]). */


#define PUNCTUATION_CHARS " \n\t,;.!?:'\"()[]{}-+*/"

#endif
