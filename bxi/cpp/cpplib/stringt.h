/*
 * File cpplib/stringt.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/stringt.h
 */

/* stringt.h -- StringTemp -- a temporary string class */
/*
	This unit defines the class StringTemp (temporary string), which is used for 2 main 
		purposes: editing strings, and writing functions that return strings.
		StringTemp objects do NOT expand themselves automatically: you have to define in 
			advance the maximum size of the string.
		However, they DO free the memory they occupy automatically when they get out 
			of scope. 
	
	  Here is an example of how to use the StringTemp class to return strings from functions:
	
		StringTemp concat_string_to_char (CStr the_string, char the_char) {
			StringTemp result;
			result.alloc ( strlen(the_string) + 2 );
			result.overwrite (the_string);
			result.append (the_char);
			return result;
		}
	
		void main () {
			StringTemp s1 = concat_string_to_char ("ABC",'1');	// s1.str = "ABC1"
			StringTemp s2 = concat_string_to_char (s1.str,'2');	// s2.str = "ABC12"
			cout << str1 << " " << str2;      // prints 'ABC1 ABC12'
				// the memory allocated for str1 and str2 will be freed automatically
		}
	
	Here is an example of how to use the StringTemp class for input:
	{
		CStr the_data;
	
		StringTemp s;
		s.alloc(200);					// The max line length is 200.
		s.readline(input);
		the_data = s.final_str();  
			// When you use final_str(), s no longer owns the data, so the data 
			//	won't be lost when s gets out of scope.
	
		freevec (the_data); // now you should free the memory manually.
	}


*/



#ifndef __STRINGT_H
#define __STRINGT_H

#include <cpplib/char2.h>
#include <cpplib/klli1.h>
#pragma warning (disable: 4270)     // disable the message: "nonstandard extension used: 'initializing': a nonconst StringTemp& must be initialized with an lvalue..."

struct StringTemp {
	char* str;
	Size size;
	Size len;

	/******* allocation and copy *********/
	status alloc (Size thesize) { len=0; return vecalloc(str,size=thesize); }
	void free () {vecfree(str); alloc(0); }
	status reset (Size thesize) { len=0; return size!=thesize? vecreset(str,size=thesize): OK; }
	void _steal (StringTemp& thestr) { str=thestr.str; size=thestr.size; len=thestr.len; thestr.alloc(0); }
	void _set (CStr thestr) { DOx(alloc(strlen(thestr)+1)); len=size-1; strcpy(str,thestr); }
	void _set (CStr thestr, Size thesize) { DOx(alloc(thesize)); len=strlen(thestr); strcpy(str,thestr); }
	
	StringTemp () { alloc(0); }
	StringTemp (Size thesize) { DOx(alloc(thesize)); }
	StringTemp (int thesize) { DOx(alloc(thesize)); }
	StringTemp (CStr thestr)   { _set(thestr); }
	StringTemp (CStr thestr, Size thesize)   { _set(thestr,thesize); }
	StringTemp (const StringTemp& thestr) {
		StringTemp& thestr_nonconst = *(StringTemp*)(&thestr);
		_steal(thestr_nonconst);
	}
	~StringTemp () { free(); }

	void steal (StringTemp& thestr) { free();  _steal(thestr); }
	void set (CStr thestr) { free(); _set(thestr); }
	void set (CStr thestr, Size thesize) { free(); _set(thestr,thesize); }
	void operator= (const StringTemp& thestr) {
		StringTemp& thestr_nonconst = *(StringTemp*)(&thestr);
		steal(thestr_nonconst);
	}

	/********** use ************/
	bool isvalid() { return str!=NULL; }
	bool isempty() { return len==0; }
	char& at (Index i) { assert(i<size); return str[i]; }
	char at (Index i) const { assert(i<size); return str[i]; }
	char& at_end (Index i) { assert(i<size); return str[len-1-i]; }
	char at_end (Index i) const { assert(i<size); return str[len-1-i]; }
	bool ends_with (char thechar) const { return at_end(0)==thechar; }
	bool ends_with (char2 thechar2) const { return at_end(0)==thechar2[1] && at_end(1)==thechar2[0]; }
	bool ends_with (CStr thestr, Size thelen) const { return strncmp(str+len-thelen,thestr,thelen)==0; }
	bool starts_with (char thechar) const { return str[0]==thechar; }
	bool starts_with (char2 thechar2) const { return str[0]==thechar2[0] && str[1]==thechar2[1]; }
	bool starts_with (CStr thestr, Size thelen) const { return strncmp(str,thestr,thelen)==0; }
	char& operator[] (Index i) { assert(i<size); return str[i]; }
	char operator[] (Index i) const { assert(i<size); return str[i]; }

	char* finalstr() { end(); char* s=str; alloc(0); return s; }
	char* duplicate() const { char* s=new char[len+1];  strcpy(s,str);  return s; }
	StringTemp clone() const { return StringTemp(duplicate(),size); }
	//StringTemp final() { char* thestr=str; Size thesize=size; alloc(0); return StringTemp(

	/********** edit **********/
	void end() { str[len]='\0'; }

	status put (Index at, CStr thestr, Index f, Index t) { // puts thestr[f..t-1] at str[at].
		if (t<f) return EINVAL;
		if (at+t-f >= size)    return ERANGE;
		memcpy (str+at, thestr+f, t-f);
		len=max (len,at+t-f);  end();
		return OK;
	}
	status put (Index at,CStr thestr) { return put (at,thestr,0,strlen(thestr)); } 
	status put (Index at,char thechar) { return put (at,&thechar,0,1); }

	status append (CStr thestr, Index f, Index t) { return put (len,thestr,f,t); }
	status append (CStr thestr) { return append (thestr,0,strlen(thestr)); } 
	status append (char thechar) { return append (&thechar,0,1); }
	status append (int i, int radix);

	status overwrite (CStr thestr) { len=0; return put(0,thestr); }

	status truncate (Index from=0) {        // truncates the string from 'from'.
		if (from>len) return ERANGE;
		len=from;    end();
		return OK;
	}
	status remove (Index from, Index to) {  // removes str[from..to-1]
		if (to==len)  return truncate (from);
		if (from>to || to>len) return ERANGE;
		strcpy (str+from, str+to); 
		len -= (to-from);
		return OK;
	}
	status remove (Index at) { return remove(at,at+1); }
	void removelast (uint howmany) { assert(len>=howmany); truncate(len-howmany); }
	void backspace (uint howmany=1) { removelast(howmany); }

	void removefirst_fast (uint howmany) { str += howmany; len -= howmany; }     // don't use this function unless you are the superuser!      
	void restorefirst (uint howmany) { str -= howmany; len += howmany; }         // don't use this function unless you are the superuser!        

	/*********** input ************/
	status read (istream& in, CStr thestoppers);        // reads characters from in, until one of thestoppers are read. the stopper is left in in.
	status read (istream& in, char thestopper) { return read (in,::str(thestopper)); }
	status readword (istream& in, CStr thestoppers=" \n\t") { return read(in,thestoppers); }
	status readline (istream& in, CStr thestoppers="\n") { DOr(read(in,thestoppers)); in.get(); return OK; }
	status readline (istream& in, char thestopper)  { return readline(in,::str(thestopper)); }

	status read_from_console (CStr the_initial_string="");
};


typedef StringTemp& StringTempR;
typedef const StringTemp& StringTempCR;

inline void write (ostream& out, StringTempCR str, Format format="") { out << str.str; }
inline status read  (istream& in,  StringTempR str, Format format="\n") { return str.readline(in,format.str()); }

inline short compare (StringTempCR a, StringTempCR b) { return strcmp(a.str,b.str); }
inline short compare (StringTempCR a, CStr b) { return strcmp(a.str,b); }
inline short compare (CStr b, StringTempCR a) { return strcmp(b,a.str); }

inline void duplicate (StringTemp& to, StringTempCR from) { to.set(from.str,from.size); }

StringTemp concat (CStr a, CStr b);    // returns a concatenated to b
StringTemp concat (CStr a, char b);    // 
StringTemp concat (char a, CStr b);    // 
#define concat3(a,b,c) concat(concat(a,b).str,c)
inline StringTemp concat (CStr a, char b, CStr c) { return concat3(a,b,c); }

StringTemp format (CStr theformat, long i);    // Example: format("ABC%dXYZ",5) will return "ABC5XYZ"
StringTemp format (CStr theformat, CStr s);    // Example: format("ABC%sXYZ","xxx") will return "ABCxxxXYZ"

StringTemp next_line (istream& in, Size thesize=400, CStr thestoppers="\n");  // returns the next line from the input (removes EOL char from the input)
StringTemp next_word (istream& in, Size thesize=30, CStr thestoppers=" \n\t");// returns the next word from the input (does NOT remove the seperator from the input)

StringTemp concat_filename_to_extension (CStr filename, CStr extension);  
StringTemp concat_path_to_filename (CStr path, CStr filename);

bool word_contains_digit (StringTempCR theword);


DEFINE_INLINE_IO_OPERATORS(StringTemp)
DEFINE_INLINE_RELATIVE_OPERATORS(StringTemp)


#endif
