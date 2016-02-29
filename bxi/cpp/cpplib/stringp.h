/*
 * File cpplib/stringp.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/stringp.h
 */

/* stringp.h -- StringParse -- a parse-string class */



#ifndef __STRINGP_H

#define __STRINGP_H



#include <cpplib/klli1.h>



struct StringParse {

	char* str;

	bool shouldfree;

	Size size;

	Size len;

	uint index;



	/******* allocation and copy *********/

	status alloc (Size thesize) { len=index=0; shouldfree=true; return vecalloc(str,size=thesize); }

	void free () { if(shouldfree) vecfree(str); alloc(0); }

	//status reset (Size thesize) { len=0; return size!=thesize? vecreset(str,size=thesize): OK; }

	void _steal (char* thestr) { str=thestr; len=strlen(thestr); index=0; size=len+1; shouldfree=false; }

	void _set (CStr thestr) { alloc(strlen(thestr)+1); len=size-1; index=0; strcpy(str,thestr); }

	

	//StringParse (Size thesize=0) { DOx(alloc(thesize)); }

	StringParse (char* thestr)   { _steal(thestr); }

	//StringParse (CStr thestr, Size thesize)   { _set(thestr); size=thesize; }

	//StringParse (StringParse& thestr) { _steal(thestr); }

	~StringParse () { free(); }



	void steal (char* thestr) { free();  _steal(thestr); }

	void set (CStr thestr) { free(); _set(thestr); }

	//void operator= (StringParse& thestr) { free();  steal(thestr); }



	/********** use ************/

	bool isvalid() { return str!=NULL; }

	bool isempty() { return len==0; }

	char at (Index i) const { assert(i<size); return str[i]; }

	char at_end (Index i) const { assert(i<size); return str[len-1-i]; }

	char operator[] (Index i) const { assert(i<size); return str[i]; }

	void reverse () { strrev(str); }



	/********* iterate **********/

	char cur(int theoffset=0) const { return at(index+theoffset); }

	char get() { if(index<len) return at(index++); else return -1; }

	void next(uint theoffset=1) { index=min(len,index+theoffset);  }

	void prev(uint theoffset=1) { if(index>theoffset-1) index-=theoffset; }

	void rewind() { index=0; }

	bool end() { return index>=len; }



	// This macro will move the index forward, until thecond is true.

	//	for example: FINDinSTRING(theline, isdigit(c)||isalpha(c) ) will find the next char

	//		which is a digit or a letter.

	#define FINDinSTRING(theline,thecond) for(; !theline.end(); theline.next()) {char c=theline.cur(); if(thecond) break; }



	// This macro will move characters from theline to thetarget, until thecond is true.

	//	For example, parse1(theline,!isdigit(c),thetarget) will parse a number.

	#define PARSE(theline,thecond,thetarget) for(; !theline.end(); theline.next()) {char c=theline.cur(); if(thecond)break; thetarget.append(c); }

};





typedef StringParse& StringParseR;

typedef const StringParse& StringParseCR;





#endif

