/*
 * File cpplib/vector1.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/vector1.hxx
 */

/* vector1.hxx -- a simple vector and a sorted vector. */

#ifndef __VECTOR1_HXX
#define __VECTOR1_HXX

#include <cpplib/klli1.h>
#include <cpplib/io2.h>

#define TEMPLATE template <class Data>
#define VECTOR Vector1<Data>
#define VECTORCR const Vector1<Data>&
#define VECTORS Vector1Sorted<Data>
#define VECTORSCR const Vector1Sorted<Data>&
#define CURSOR Vector1Cursor<Data>
#define CURSORCR const CURSOR&

/**************************************************************************
	A Vector1 is a simple type, encapsulating the C++ pointer. 
	It also remembers it's allocated size, and performs a simple boundary-check.
	The Vector1 DOES NOT free itself automatically when it's use is over (you should free it explicitly):
	
	Here is a simple usage example:
	void main (void) {
		Vector1<int> myvector(4);
		LOOPVECTOR (;,myvector,i)   // equivalent to: for (Index i=0; i<myvector.count(); ++i)
			myvector[i]=i;
		writeln (cout,myvector);            // prints: 0123
		free (myvector);
	}
	
	

	A Vector1 has many functions that can be used to insert, remove or replace
		the elemepts in the vector. The most basic function is:
			replace (from,to, with,f,t)
		which replaces a section of this string [from..to-1] with a section of
		'with[f..t-1]'. You can use it to insert (by taking from==to), 
		remove (by taking with==""), and any other use...

    A Vector1Sorted is a simple extension of Vector1, which sorts its elements
		using the function 'compare' (you must define this function for the 
		type Data).
**************************************************************************/


/**************************************************************************/
/**********        Vector1                            ***********************/
/**************************************************************************/

TEMPLATE class Vector1Cursor;
TEMPLATE class Vector1;

template <class Data_> class Vector1 {
public:		
	typedef Data_ Data;
	typedef CURSOR Cursor;

protected:
	Pointer1<Data> myinfo;

public:		
	/***********  construction and destruction **********/

	Vector1 (Size the_size=0)  { DOx(myinfo.alloc(the_size));  }
	~Vector1() { /* free();*/  } // nothing is done automatically!

	void free() { myinfo.free(); }
	void remove_all() { myinfo.free(); }




	/* this function resizes the array and loses the old myinfo.p */
	status realloc(Size the_newsize) { 
		Data* old_elements; 
		DOr (myinfo.realloc (the_newsize,old_elements));
		vecfree(old_elements);	
		return OK;
	}

	status reset(Size the_newsize) { return realloc(the_newsize); }

	
	/* this function resizes the array and keeps the old elements */
	status resize (Size the_newsize) {
		Data* old_elements;
		Size the_oldsize = myinfo.size;
		DOr (myinfo.realloc(the_newsize,old_elements));
		if (old_elements==NULL) return OK; // no copy needed
		DOr(myinfo.copy(0,old_elements, min(the_oldsize,the_newsize) ));
		vecfree(old_elements);
		return OK; 
	}

	status resize_if_needed (Size the_minimum_newsize, int thedelta=1) {
		return resize (newsize(myinfo.size,the_minimum_newsize,thedelta)); }

	status append_empty() { return resize_if_needed (myinfo.size+1); }


	/**************      assignment functions     ***********************/
	status share (VECTORCR from) { return myinfo.share(from.myinfo);	}
	status steal (VECTOR& from)  { return myinfo.steal(from.myinfo); }
	void operator= (VECTORCR from) { share(from); }



	/****************              query              *******************/
	Index size() const { return myinfo.size; }
	Index count() const { return myinfo.size; }
	bool isempty() const { return myinfo.p==NULL; }
	DataCR last() const { return myinfo[myinfo.size-1]; }
	DataR  last() { return myinfo[myinfo.size-1]; }

	Data* buffer() { return myinfo.p; } 
	const Data* buffer() const { return myinfo.p; }

	DataR at (Index index)		{ return myinfo[index];      }
	DataCR at (Index index) const	{ return myinfo[index];      }
	DataR operator[] (Index index)	     { return at(index); }
	DataCR  operator[] (Index index) const  { return at(index); }


	status append (DataCR thedata) { 
		Size old_size = myinfo.size;
		Size new_size = old_size + 1;
		Data* old_elements;   myinfo.realloc(new_size, old_elements);
		if (old_elements!=NULL) myinfo.copy (0, old_elements, old_size);  // copy the first part
		myinfo[old_size]=thedata;		 
		vecfree(old_elements);
		return OK;
	}


	/*************** replace ************************************/
	/***** replaces myinfo.p[from..to-1] with with[f..t-1] ******/
	status replace (Index from, Index to, const Data* with, Index f, Index t) {
		sort2(from,to);  sort2(f,t);
		if (to>myinfo.size) return ERANGE;    /* ERROR */
		Size old_size = myinfo.size;
		Size replaced_size = to-from;
		Size replacing_size = t-f;
		Size new_size = old_size - replaced_size + replacing_size;
		if (new_size==old_size) {          // copy over the replaced section
			myinfo.copy (from, with+f, replacing_size);
		}
		else {
			Data* old_elements;  
			myinfo.realloc(new_size, old_elements);
			if (old_elements!=NULL) myinfo.copy (0, old_elements, from);  // copy the first part
			myinfo.copy (from, with+f, replacing_size);		 // copy the middle part 
			myinfo.copy (to-replaced_size+replacing_size, old_elements+to, old_size-to); // copy the last part
			vecfree(old_elements);
		}	
		return OK;
	}

	status replace (Index from, Index to, VECTORCR with, Index f, Index t) { return replace (from,to,with.myinfo.p, f,t ); }
	status replace (Index from, Index to, VECTORCR with) { return replace (from,to,with.myinfo.p,0,with.size() ); }

	/* replace a segment by a single Data */
	status replace (Index from, Index to, DataCR with) { return replace (from,to, &with, 0,1); }

	/* insert */
	status insert (Index at, const Data* s, Index f, Index t) { return replace(at,at,s,f,t); }
	status insert (Index at, VECTORCR s, Index f, Index t) { return replace(at,at,s,f,t); }
	status insert (Index at, VECTORCR s) { return replace(at,at,s); }
	status insert (Index at, DataCR c) { return replace(at,at,c); }
	status insert (DataCR c) { return insert (myinfo.size,c); }

	/* overwrite */
	status overwrite (Index at, const Data* s, Index f, Index t) { return replace (at,min(myinfo.size,at+t-f), s,f,t); }
	status overwrite (Index at, DataCR c) { return overwrite (at,&c,0,1); }
	status overwrite (Index at, VECTORCR with) { return overwrite (at,with.myinfo.p,0,with.size() ); }

	/* add at the end */
	status append (VECTORCR s) { return insert (myinfo.size,s); }
	//status append (DataCR c) { return insert (myinfo.size,c); }

	/* add at the beginning */
	status prepend (VECTORCR s) { return insert (0,s); }
	status prepend (DataCR c) { return insert (0,c); }

	/* assign a string */
	status assign (const Data* s, Index f, Index t) { return replace (0,myinfo.size,s,f,t); }
	status assign (VECTORCR s, Index f, Index t) { return assign (s.myinfo.p,f,t); }
	status assign (VECTORCR s) { return assign (s.myinfo.p,0,s.size() ); }

	/* remove a segment */
	status removeat (Index from, Index to) { return replace (from,to,myinfo.p,0,0); }
	status removeat (Index index) { return removeat(index,index+1); }
	status truncate (Index i=0) { return removeat (i,myinfo.size); }
	

	bool operator==(int ZERO) const { return isempty(); }
	void operator= (int ZERO) { free(); }

	void duplicate (VECTORCR from) { ::duplicate(myinfo,from.myinfo); }
};

TEMPLATE void duplicate (VECTOR& to, VECTORCR from) {
	to.duplicate(from);
}

TEMPLATE void free (VECTOR& v) {
	v.free();
}



/**************************************************************************/
/**********        Vector1Cursor                            ***********************/
/**************************************************************************/

TEMPLATE class Vector1Cursor { 
	protected:
		VECTOR* myvector;
		Index myindex;
	public:

		/* queries */
		bool isvalid() const { return myindex < myvector->size(); } 
		bool isnextvalid() const { return myindex+1 < myvector->size(); } 
		DataCR item() const  { return myvector->at(myindex); }
		Data&  item()		 { return myvector->at(myindex); }
		Index index() const  { return myindex; }
		
		/* use */
		void reset (VECTOR& thevec) { myvector=&thevec;  myindex=0; }
		void assign (CURSORCR c) { myvector=c.myvector;  myindex=c.myindex; }

		Vector1Cursor (VECTOR& thevec) { reset(thevec); }
		Vector1Cursor (CURSORCR thecursor) { assign(thecursor); }
		Vector1Cursor (Index theindex=0) { myindex=theindex; }

		void operator= (CURSORCR thecursor) { assign(thecursor); }

		void next() { ++myindex; }
		void operator++() { next(); }

		const CURSOR* clone() const { return new CURSOR (myindex); }
};


/* find:
	This function searches the vector for the first occurence of the item.
	It searches from 'from' to 'to'-1, and returns the result in theresult.
	In order to use this function, the function 'compare' must be defined
		for the type Data (it is already defined for the basic C types,
		in the file "cpplib/typedefs.h").
*/
TEMPLATE status find (Data theitem, VECTORCR thevec, Index from, Index to,
					  Index& theresult) 
{
	sort2 (from,to);
	if ( to > thevec.size() ) return ERANGE;

	for (Index i=from;  i<to;  ++i)
		if ( compare (thevec[i],theitem) == 0 ) {
			theresult=i;
			return OK;
		}
	return ENOTFOUND;
}	

TEMPLATE status find (Data theitem, VECTORCR thevec, Index from, Index& theresult) 
	{ return find (theitem,thevec, from,thevec.size(), theresult); }

TEMPLATE status find (Data theitem, VECTORCR thevec, Index& theresult) 
	{ return find (theitem,thevec, 0u,thevec.size(), theresult); }

TEMPLATE bool contains (Data theitem, VECTORCR thevec) {
	Index theresult;
	return find(theitem,thevec,theresult)==OK;
}

/* findp / findcp ('c' for 'const') -- 
	returns a pointer to the item in the vector, or NULL if not found.
*/

TEMPLATE const Data* findcp (Data theitem, VECTORCR thevec)
{
	Index theindex;
	if ( find(theitem,thevec,theindex) == ENOTFOUND)
		return NULL;
	else
		return &thevec[theindex];
}
TEMPLATE Data* findp (Data theitem, VECTOR& thevec)
{
	Index theindex;
	if ( find(theitem,thevec,theindex) == ENOTFOUND)
		return NULL;
	else
		return &thevec[theindex];
}



TEMPLATE status find (Data theitem, CURSOR& thecursor)
{
	for (; thecursor.isvalid(); ++thecursor)
		if ( compare (thecursor.item(),theitem) == 0 ) 
			return OK;
	return ENOTFOUND;
}	



/* find_and_remove --
	Finds the item in the vector and removes it.
	Returns OK if found and ENOTFOUND if not.
*/
TEMPLATE status find_and_remove (Data theitem, VECTOR& thevec) 
{
	Index theindex;
	DOr (find (theitem, thevec, theindex));
	return thevec.removeat(theindex);
}



/************************************************************************************************/
/************  	IO operations      **************************************************************/
/************************************************************************************************/


/* 
	The output format should contain at least one char, which is the item-separator.
	If the format is preceded by 'P', it says that the output will be surrounded
		by parenthesis.
	If the format is preceded by '*', it will be skipped.
	examples: (the vector  contains a,b,c and d)
		"P "           ==> [4: a b c d]
		"P,"           ==> [4: a,b,c,d]
		" "  (or "* ") ==> a b c d
		","  (or "*,") ==> a,b,c,d
*/
TEMPLATE void write (ostream& out, VECTORCR thevec, Format format="") {
	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]
		format = format.next();
		out << '[';
		out << thevec.count() << ": ";
		write_vector (out,thevec, str(format[0]),format.next() );
		out << ']';
	}
	else {                          // Simple format: a1 a2 ... an
		if (format[0]=='*' || format[0]=='L')  format=format.next();
		write_vector (out,thevec, str(format[0]),format.next() );
	}
}

//TEMPLATE void write (ostream& out, ARRAYCR thevec) { write (out,thevec,""); }

TEMPLATE status read (istream& in, VECTOR& thevec, Format format="") {
	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]
		format = format.next();
		DOr(testchar(in,'['));
		Size thelen=0;
		DOr(read(in,thelen));
		DOr(testchar(in,':'));
		DOr(thevec.realloc(thelen));
		return read_vector (in,thevec, /*separator*/format[0], /*stopper*/']', format.next() );
	}
	else if (format[0]=='L')    {   // line format: read myinfo.p until end-of-line
		format = format.next();
		thevec.truncate();
		return read_container (in,thevec, format[0], '\n', format.next()); 
	}
	else {                          // Simple format: a1 a2 ... an (until the EOF or another error).
		if (format[0]=='*')  format=format.next();
		thevec.truncate();
		return read_container (in,thevec, format[0], '\0', format.next()); 
	}
}


/**************************************************************************/
/**********        VectorSorted                     ***********************/
/**************************************************************************/

TEMPLATE class Vector1Sorted: public VECTOR {
	protected:

		/* find_range -- 
			Looks for theitem in thevector, and if it is not found -- returns the range of indices
				in which it should be placed, under the assumption that the vector is sorted.
				(it is used to know where to insert a new item in a sorted vector).
			(If the item is not in the vector, theupper=thelower+1, since the item can be placed
				between the upper and the lower indices).
			If the item is larger than the last item in the vector, theupper will be thevec.size().
			If the item is smaller than the first item in the vector, thelower will be -1.
			(If the vector is empty, theupper=0 and thelower=-1);
		*/
		status find_range (DataCR theitem, 	int& thelower, int& theupper) 
		{
			int lower=-1, upper=size();
			for (;;) {
				if ( lower+1 >= upper ) {
					theupper=upper; thelower=lower;
					return OK;
				}
				Index middle = (lower+upper) / 2;
				DataCR themedian = at(middle);
				short c = compare(theitem,themedian);
				if      (c<0)  upper=middle; // theitem<themedian
				else if (c>0)  lower=middle; // theitem>themedian
				else           { thelower=middle-1;  theupper=middle+1; return OK; }  // theitem==themedian
			}
		}

	public:
		Vector1Sorted (Size thesize=0): VECTOR(thesize) { }

		status find (DataCR theitem, Index& theindex) const
		{
			int lower=-1, upper=size();
			for (;;) {
				if ( lower+1 >= upper ) return ENOTFOUND;
				Index middle = (lower+upper) / 2;
				DataCR themedian = at(middle);

				short c = compare(theitem,themedian);
				if      (c<0) upper=middle; // theitem<themedian
				else if (c>0) lower=middle; // theitem>themedian
				else          { theindex=middle; return OK; }  // theitem==themedian -- found!
			}
		}

		const Data* findp (DataCR theitem) const {
			Index theindex;
			if (find(theitem,theindex) == ENOTFOUND) return NULL;
			else return &at(theindex);
		}
		Data* findp (DataCR theitem) {
			Index theindex;
			if ( find(theitem,theindex) == ENOTFOUND) return NULL;
			else return &at(theindex);
		}

		bool contains (DataCR theitem) const {
			return findp(theitem)!=NULL;
		}


		/* insert -- 
			inserts the item in the vector, just before the first element which is larger than it.
		*/
		status insert (DataCR theitem) {
			int lower, upper;
			find_range (theitem, lower, upper);
			return VECTOR::insert (Index(upper),theitem);
		}

		status insert_unique (DataCR theitem) {
			int lower, upper;
			find_range (theitem, lower, upper);
			if (upper-lower >= 2) return EEXIST;        // item already exists!
			return VECTOR::insert (Index(upper),theitem);
		}

		status remove (DataCR theitem) {	
			Index theindex;
			DOr (find(theitem, theindex));
			return removeat(theindex);	
		}

};


/* find_and_remove --
	Finds the item in the vector and removes it.
	Returns OK if found and ENOTFOUND if not.
*/
TEMPLATE status find_and_remove (DataCR theitem, VECTORS& thevec) { 
	return thevec.remove (theitem); }

TEMPLATE bool contains (VECTORSCR thevec, Data theitem) {
	return thevec.contains(theitem); }

TEMPLATE const Data* findcp (DataCR theitem, VECTORSCR thevec) {
	Index theindex;
	if ( thevec.find(theitem,theindex) == ENOTFOUND) return NULL;
	else return &thevec[theindex];
}
TEMPLATE Data* findp (DataCR theitem, VECTORS& thevec) {
	Index theindex;
	if ( thevec.find(theitem,theindex) == ENOTFOUND) return NULL;
	else return &thevec[theindex];
}


#undef TEMPLATE
#undef VECTOR
#undef VECTORCR
#undef CURSOR 
#undef CURSORCR
#endif

