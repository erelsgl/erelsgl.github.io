/*
 * File cpplib/stack.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/stack.hxx
 */

/* stack.hxx -- a simple stack */
/*
	This unit defines the template-class Stack (see below).
*/


#ifndef __STACK_HXX
#define __STACK_HXX

#include <cpplib/klli1.h>

#define TEMPLATE template <class Data>
#define STACK Stack<Data>
#define STACKR STACK&
#define STACKCR const STACK&


/**************************************************************************
	A Stack is a simple data structure implemented as a vector.
	The size of the stack is FIXED -- you should allocate enuogh space in advance.
	The stack will free itself automatically after use.
**************************************************************************/

TEMPLATE struct Stack {
	Data* myinfo;
	Size mysize;     
	Index mytop;

	/******* allocation  *********/
	status alloc (Size thesize) { mytop=0; return vecalloc(myinfo,mysize=thesize); }
	void free () {vecfree(myinfo); alloc(0); }
	status reset (Size thesize) { mytop=0; return mysize!=thesize? vecreset(myinfo,mysize=thesize): OK; }
	
	Stack (Size thesize=0) { DOx(alloc(thesize)); }
	~Stack () { free(); }

	/********** queries ************/
	bool isvalid() const { return myinfo!=NULL; }
	bool isempty() const { return mytop==0; }
	bool isfull()  const { return mytop==mysize; } 

	/********** actions ************/
	status push (DataCR d) {
		if (isfull()) return EFULL;
		myinfo[mytop++]=d;
		return OK;
	}

	DataCR top() const { assert(!isempty());  return myinfo[mytop-1]; }

	status pop(DataR result) { 
		if (isempty()) return EEMPTY;
		result = myinfo[--mytop];
		return OK;
	}

	DataCR pop () { assert (!isempty()); return myinfo[--mytop]; }

	void truncate() { mytop=0; }

};


#undef TEMPLATE
#endif
