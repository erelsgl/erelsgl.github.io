/*
 * File cpplib/mem.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/mem.h
 */

/* mem.h */
/*
	This unit defines standard memory-management routines:
		status vecalloc (the_pointer, the_size) -- allocate vector (+ handle errors)
		void   vecfree  (the_pointer) -- free vector (+ handle errors)
		status vecreset (the_pointer, the_new_size) -- frees the old vector and allocate new one (+ handle errors)
	
	It also defines the function:
			Size newsize (Size the_current_size, Size the_minimum_new_size, int the_delta)
		which is used in arrays that expand themselves automatically.
	
	It also defines the template class Pointer1, which encapsulates the operations on 
		pointers, and also remembers the allocated size and performs boundary-checking.
*/



#ifndef __MEM_H
#define __MEM_H

//#define _CRTBLD
//#include <dbgint.h>

#undef TEMPLATE
#define TEMPLATE template <class Data>
#define POINTER Pointer1<Data>
#define POINTERCR const POINTER&

/*************************** allocation functions ******************************/

TEMPLATE  status vecalloc (Data* & thepointer, Size size) {
	if (size==0) {
		thepointer=NULL;
		return OK;
	}
	else {
		DOPr(thepointer = new Data[size]);
		return OK;
	}
}


TEMPLATE  void vecfree (Data* &thepointer) {
	if (thepointer!=NULL)  { delete[] thepointer;  thepointer=NULL; }
}


TEMPLATE  status vecreset (Data* &thepointer, Size the_newsize) {
	vecfree (thepointer);
	return vecalloc (thepointer, the_newsize);
}


/* newsize -- returns the new size of this vector, so that its size will be at least the minimum. */
/*	If the size should be changed, it will be changed by thedelta. */
/*  Use thedelta=-1 for an exponential growth (*2+1) */
inline Size newsize (Size mysize, Size the_minimum_newsize, int thedelta) {
	Size newsize = mysize;
	if (thedelta==-1)     // exponential growth
		for (; newsize<the_minimum_newsize; newsize = newsize*2 + 1);
	else
		for (; newsize<the_minimum_newsize; newsize = newsize+thedelta);
	return newsize;
}




/*************************************************************************/
/*************************** class Pointer1 ******************************/
/*************************************************************************/
/* 
	A Pointer1 is like a regular pointer, but it also remembers it's size,
		so it can perform boundary-checking.
*/


TEMPLATE  struct Pointer1 {
	Data* p;
	Size size;

	status alloc (Size thesize) { return vecalloc (p, size=thesize); }
	void free() { vecfree(p); size=0; }
	status reset (Size thenewsize) { 
		if (thenewsize!=size) return vecreset(p,size=thenewsize);
		else return OK;
	}
	status realloc (Size thenewsize, Data* &old_p) {  // old_p returns the old pointer (NULL if not replaced).
		if (thenewsize!=size) {
			old_p=p;
			return vecalloc(p,size=thenewsize);
		}
		else {
			old_p=NULL;
			return OK;
		}
	}
	status mrealloc (Size theminnewsize, int thedelta, Data* &old_p) { return realloc(newsize(size,theminnewsize,thedelta), old_p); }

	Pointer1 (Size the_size=0) { DOx(alloc(the_size));  }

	operator Data*() { return p; }
	operator const Data*() const { return p; }

	DataR at (Index index)		{ assert (index<size); 	return p[index];    }
	DataCR at (Index index) const	{ assert (index<size);  return p[index];    }
	DataR operator[] (Index index)	     { return at(index); }
	DataCR  operator[] (Index index) const  { return at(index); }


	/****** deep-copy routines ***********/
	status copy (Index at, const Data* from, Size howmany) {
		if (at+howmany > size)  return ERANGE;
		memcpy (p+at, from, howmany*sizeof(Data));
		return OK;
	}

	/********** shallow-copy routines *************/
	status share (const Pointer1<Data>& from) { p=from.p;   size=from.size; return OK;	}
	status steal (Pointer1<Data>& from)  { share(from);  from.p=NULL; from.size=0; return OK; }
	void operator= (const Pointer1<Data>& from) { share(from); }

	bool isvalid () const { return true; }
	//	if (p==NULL) return true;
	//	_CrtMemBlockHeader* pHead = pHdr(p);
	//  return (_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));


};


TEMPLATE void duplicate (POINTER& to, POINTERCR from) {
	DOx ( to.reset(from.size) ); 
	for (Index i=0; i<from.size; ++i)
		duplicate (to.p[i],from.p[i]);
}

TEMPLATE void free (POINTER& p) {
	for (Index i=0; i<p.size; ++i)
		free (p.p[i]);
	p.free();
}

	

#undef TEMPLATE
#endif
