/*
 * File cpplib/klli1.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/klli1.cpp
 */

/* klli1.cpp -- general routines */



#include <cpplib/klli1.h>
#include <stdlib.h>
#include <fcntl.h>

status the_status;

void* the_pointer;

char* error_messages[] = {	"","operation not permitted","no such file or directory","no such process","interrupted function call",
							"input/output error","no such device or address","arg list too long","exec format error","bad file descriptor",
							"no child process","resource temporarily unavailable","not enough memory","permission denied","bad address",
							"unknown error","resource device","file exists","improper link","no such device",
							"not a directory","is a directory","invalid argument","too many open files in system","too many open files",
							"inappropriate I/O control operation","unknown error","file too large","no space left on device","invalid seek",
							"read only file system","too many links","broken pipes","domain error","result too large",
							"unknown error","resource deadlock avoided","unknown error","filename too long","no locks available",
							"function not implemented","directory not empty","illegal byte sequence","structure is empty","element not found",
							"bad input format","structure is full"
};




int array2_delta=-1;

CStr str (char c)
{
	static char temporary_string[52]="a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z";       // used to hold a string that represents a char 
	static Index cur_str = 0;
	cur_str = (cur_str+2) % (sizeof temporary_string);
	temporary_string[cur_str]=c;
	return &temporary_string[cur_str];
}




void exit_with_error (const char* error_type, const char* argument, const char* synopsis/*=NULL*/)
{
	cerr << error_type << ": " << argument << endl;
	if (synopsis!=NULL)
		 cerr << "SYNOPSIS: " << synopsis << endl;
	exit (1);
}

/*
#include <io.h>
long filesize (char* path) {
	int handle = open(path,O_RDONLY);
	long size = filelength(handle);
	close (handle);
	return size;
}
*/

status open (CStr name, ifstream& file, open_mode or_mode /*=ios::in*/) {
	if (file.is_open()) file.close(); 
	if (name==NULL)  { /*file.sync_with_stdio();*/ file.open("con"); return OK; }
	//file.open (name, (ios::in | or_mode));
	file.open (name, ios::in);
	if (file.fail()) return ENOTFOUND;
	return OK;
}

status open (CStr name, ofstream& file, open_mode or_mode /*=ios::app*/) {
	if (file.is_open()) file.close(); 
	if (name==NULL)  { /*file.sync_with_stdio();*/ file.open("con"); return OK; }
	//file.open (name, (ios::out | or_mode));
	if (or_mode==ios::trunc)
		file.open (name, ios::out|ios::trunc);
	else 
		file.open (name, ios::out|ios::app);
	if (file.fail()) return EACCES;
	return OK;
}


char case_convert (uchar source, uchar source_zero, uchar target_zero)
{
	if (source_zero <= source && source < source_zero+32)
		return (source - source_zero + target_zero);
	else
		return source;
}

char parenthesis_reverse (char source)
{
	switch (source) {
		case '(': return ')';
		case ')': return '(';
		case '[': return ']';
		case ']': return '[';
		case '{': return '}';
		case '}': return '{';
		default:  return source;
	}
}

void skip (istream& in, CStr thechars) { // example: skip(cin," \t\n") skips whitespace in the standard input.
	for(;;) {
		char c = in.peek();
		if ( !charinstring(c,thechars) ) break;		
		else /*cerr << "S" <<*/ in.get();
	}
}

void skipto (istream& in, CStr thechars) { // example: skip(cin,"\n") goes to the end of the line.
	for(;;) {
		if (in.eof()) break;
		if ( charinstring(in.get(),thechars) ) break;
	}
}

void seek (istream& in, CStr thechars) { 
	for(;;) {
		if (in.eof()) break;
		if ( charinstring(in.peek(),thechars) ) break; 
		else in.get();
	}
}


void skip_comments (istream& in, char comment_char) {
	for (;;) {
		skip (in,"\n\t ");
		if (in.peek()==comment_char)
			skipto (in,"\n");
		else break;
	}
}


status testchar (istream& in, char c) 
{
	if (c=='\0') return OK;
	if (c=='\n') { skip(in,"\t "); }
	else if (c!=' ') { in>>ws; }
	if ( in.peek() != c ) 
		return EBADFORMAT; 
	else {
		in.get(); 
		return OK;
	}
}



void write_str (ostream& out, CStr the_str, int from, int to) {
// write the_str[from..to-1]. if 'from' or 'to' are negative, they meen 'from the end of the string'.
	uint the_len = strlen(the_str);
	if (from<0) from += the_len;	
	if (to<=0) to += the_len;
	for (Index i=(Index)from; i<(Index)to; ++i)
		out << the_str[i];
}
	
