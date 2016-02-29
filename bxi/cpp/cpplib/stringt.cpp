/*
 * File cpplib/stringt.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/stringt.cpp
 */

/* stringt.cpp -- StringTemp -- a temporary string class */

#include <cpplib/stringt.h>


/****************************************************************/
/*******           IO Functions                ******************/
/****************************************************************/

status StringTemp::read (istream& in, CStr stoppers) {
	if (!isvalid()) return EINVAL;
	// Reads characters from 'in' to this string,  until one of the chars in format is read.
	//  (The chars in format are treated as delimiters). the delim is left in in.
	for (len=0;;++len) {
		if (len+1>=size) { end(); return ERANGE; }
		if (contains(stoppers,in.peek())) { end(); return OK; }
		if (in.eof()) { end(); return EOF; }
		str[len] = char(in.get());
	}
}

status StringTemp::append (int i, int radix) { 
#ifdef UNIX
	if (radix!=10)
		return EINVAL;   // radix != 10 not supported.
	sprintf(str+len,"%d",i);
#else
	itoa(i,str+len,radix); 
#endif
	return OK; 
}


#ifdef UNIX
status StringTemp::read_from_console (CStr the_first_string) {
	if (!isvalid()) return EINVAL;
	cin >> str;
	return OK;
}

#else
#include <conio.h>
#define cputs _cputs
status StringTemp::read_from_console (CStr the_first_string) {
	if (!isvalid()) return EINVAL;
	overwrite (the_first_string);
	cputs (the_first_string);
	char c;
	Index cursor;
  begin:
	cursor=len;
	for (;;) {        // loop until ESC or \ or EOL is pressed
		c=toupper(getch());
		if (c=='\n'||c=='\r') { end(); return OK; }
		if (c==ESC)  { // cancel string
			for (Index i=0; i<len; ++i) 
				cputs("\b");
			len=0;
			goto begin;  
		}
		if (c=='\b') {					   // backspace	
			if (len > 0) {
				cputs ("\b \b");
				--len;
			}
			continue;	
		}		   
		if (c==-32) {          // control sequence
			c=getch();
			continue;   //ignore
			if (c==75) {         // left
				cputs("\b");
				--len;
			}
			else if (c==77) {      //right
				if (len+1>=size) { end(); return ERANGE; }
				putch(str[len++]);
			}
			continue;
		}
		if (len+1>=size) { end(); return ERANGE; }
		if (0<=c && c<=32)  continue;    // non-printable char
		putch(str[len++]=c);   
	}
}
#endif


StringTemp next_line (istream& in, Size thesize, CStr stoppers) {
	StringTemp theresult(thesize);
	theresult.readline(in,stoppers);
	return theresult;
}

StringTemp next_word (istream& in, Size thesize, CStr stoppers) {
	StringTemp theresult(thesize);
	theresult.readword(in, stoppers);
	return theresult;
}




/****************************************************************/
/*******           String Functions            ******************/
/****************************************************************/

StringTemp concat (CStr a, CStr b)  {
	StringTemp thesum (strlen(a)+strlen(b)+1);
	thesum.append(a);
	thesum.append(b);
	return thesum;
}

StringTemp concat (CStr a, char b)  {
	StringTemp thesum (strlen(a)+2);
	thesum.append(a);
	thesum.append(b);
	return thesum;
}

StringTemp concat (char a, CStr b)  {
	StringTemp thesum (strlen(b)+2);
	thesum.append(a);
	thesum.append(b);
	return thesum;
}

StringTemp format (CStr theformat,long i) {    // Example: format("ABC%dXYZ",5) will return "ABC5XYZ"
	StringTemp theresult (strlen(theformat)+10);
	sprintf (theresult.str,theformat,i);
	return theresult;
}

StringTemp format (CStr theformat, CStr s) {    // Example: format("ABC%sXYZ","xxx") will return "ABCxxxXYZ"
	StringTemp theresult (strlen(theformat)+strlen(s));
	sprintf (theresult.str,theformat,s);
	return theresult;
}



void concat_filename_to_extension (StringTemp& target, CStr filename, CStr extension)
{
	target.overwrite (filename);
	if ( extension[0] != '.' )
		target.append ('.');
	target.append (extension);
}

StringTemp concat_filename_to_extension (CStr filename, CStr extension) {
	StringTemp result (strlen(filename)+strlen(extension)+2);
	concat_filename_to_extension (result,filename,extension);
	return result;
}

void concat_path_to_filename (StringTemp& target, CStr path, CStr filename)
{
	target.overwrite (path);
	if ( target.at_end(0) != '/')
		target.append ('/');
	target.append (filename);
}

StringTemp concat_path_to_filename (CStr path, CStr filename)  {
	StringTemp result (strlen(filename)+strlen(path)+2);
	concat_path_to_filename (result,path,filename);
	return result;
}


bool word_contains_digit (StringTempCR theword) {
	for (Index i=0; i<theword.len; ++i)
		if (isdigit(theword[i])) return true;
	return false;
}


#ifdef TEST_STRINGT
#include <cpplib/ui.h>

void main (void) {
	StringTemp s (20);
	s.append ('a');
	s.append ("bc");
	cout << s.str << endl;                                   // abc
	cout << sum ( sum("AA","BBCC").str, "DD" ) << endl;      // AABBCCDD
	s = input_string ("Please enter your name",30);
	cout << "your name is " << s << endl;
	int i = input_int ("Enter your age",0,200);
	cout << "your age is " << i << endl;
}

#endif
