/*
 * File cpplib/array2.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/array2.hxx
 */

/* array2.hxx -- an array container */

#ifndef __ARRAY2_HXX
#define __ARRAY2_HXX

#include <cpplib/klli1.h>
#include <cpplib/io2.h>

extern int array2_delta;        // defined in klli1.cpp
// delta is used to control resize-policy. (use delta=-1 for exponential growth).


/* Here is an Array2 usage example:

	void main (void) {
		Array2<int> myarray;
		myarray.append(0);
		myarray.append(2);
		myarray.append(1);
		writeln (cout,myarray,Format("P"));        // prints  [3: 0 2 1]
		LOOPVECTOR(;,myarray,i)                    // equivalent to: for(Index i=0; i<myarray.count(); ++i)
			myarray[i]++;
		writeln (cout,myarray,Format("P"));        // prints  [3: 1 3 2]
		free (myarray);
	}
*/


#define TEMPLATE template <class Data>
#define ARRAY Array2<Data>
#define ARRAYR ARRAY&
#define ARRAYCR const ARRAY&
#define CURSOR Array2Cursor<Data>
#define CURSORR CURSOR&
#define CURSORCR const CURSOR&

/**************************************************************************
**************************************************************************/


/**************************************************************************/
/**********        Array2                            ***********************/
/**************************************************************************/

template <class Data_> class Array2 {
	public:		
		typedef Data_ Data;
	protected:
		Pointer1<Data> myinfo;
		Index mylen;

	public:		
		/************ allocation functions ************/

		status alloc (Size size) { mylen=0; return myinfo.alloc(size); }


		/***********  construction and allocation **********/
		Array2 (Size the_size=0) { DOx(alloc(the_size));  }
		status reset (Size thenewsize) { mylen=0; return myinfo.reset(thenewsize);  }
		status realloc (Size newlen) {
			Data* old_p;  return myinfo.mrealloc(mylen=newlen,array2_delta,old_p);
			vecfree (old_p);
		}
		status resize (Size newlen) {
			Size myoldlen=mylen;   mylen=newlen;
			Data* old_p;  DOr(myinfo.mrealloc(mylen,array2_delta,old_p));
			if (old_p==NULL)  return OK;
			myinfo.copy(0,old_p,myoldlen);
			vecfree (old_p);
			return OK;
		}


		/****************  query      *******************/
		Index size() const { return myinfo.size; }
		Index len() const { return mylen; }
		Index count() const { return mylen; }
		Index lastindex() const { return len()-1; }
		bool isempty() const { return len()==0;; }

		DataR operator[] (Index index)	     { return myinfo[index]; }
		DataCR  operator[] (Index index) const  { return myinfo[index]; }
		DataR at (Index index)	     { return myinfo[index]; }
		DataCR  at (Index index) const  { return myinfo[index]; }
		DataR last() { return myinfo[mylen-1]; }


		/*************** edit **************************/
		status replace (Index from, Index to, DataCR thedata) {
			if (from==to) return insert (from,thedata);
			sort2(from,to);  if (to>mylen) return ERANGE; 
			if (from+1==to) { myinfo[from]=thedata; return true; }
			Size howmany = to-from;   // howmany >= 2
			myinfo[from] = thedata;
			myinfo.copy(from+1, myinfo.p+to, mylen-to);
			mylen -= (howmany-1);
			return OK;
		}		

		status insert (Index at, DataCR thedata) {
			if (at>mylen) return ERANGE;
			DOr (resize(mylen+1));
			myinfo.copy(at+1, myinfo.p+at, mylen-at-1);
			myinfo[at] = thedata;
			return OK;
		}

		status append_empty () {              // appends an empty slot. increases mylen.
			return resize(mylen+1);
		}

		status append (DataCR thedata) { 
			DOr(resize(mylen+1));
			myinfo[mylen-1] = thedata;
			return OK;
		}

		status append (Array2<Data> thearray) {
			Size myoldlen=mylen;
			DOr(resize(mylen+thearray.mylen));
			LOOPVECTOR(;,thearray,i)
				myinfo[myoldlen+i] = thearray.myinfo[i];
			return OK;
		}

		status removeat (Index from, Index to) { 
			sort2(from,to);  if (to>mylen) return ERANGE; 
			Size howmany = to-from;
			if (to<mylen) 
				myinfo.copy (from, myinfo.p+to, mylen-to);
			mylen -= howmany;
			return OK;
		}
		status removeat (Index at) { 
			if (at+1>mylen) return ERANGE;
			if (at+1<mylen)
				myinfo.copy (at, myinfo.p+at+1, mylen-at-1);
			mylen -= 1;
			return OK;
		}
		status truncate (Index at=0) { mylen=at; return OK; }
		status removelast () { mylen--; return OK; }
		void remove_all() { truncate(); }

		/* assignment functions */
		status share (ARRAYCR from) { mylen=from.mylen;  return myinfo.share(from.myinfo); }	
		status steal (ARRAYR from)  { mylen=from.mylen; from.mylen=0; return myinfo.steal(from.myinfo); }
		void operator= (ARRAYCR from) { share(from); }
					
		bool operator==(int ZERO) const { return isempty(); }
		void operator= (int ZERO) { remove_all(); }

		void duplicate (ARRAYCR from) {
			if (size() < from.len()) {
				free ();
				DOx(reset ( from.len() ));
			}
			mylen = from.len();
			for (Index i=0; i<from.len(); ++i)
				::duplicate (at(i), from[i]);
		}

		void free () {
			for (Index i=0; i<len(); ++i)
				::free (at(i));
			myinfo.free(); mylen=0;
		}

		bool sharing (ARRAYCR b) const {
			return myinfo.p==b.myinfo.p;
		}

		bool isvalid() const { return myinfo.isvalid(); }
};


TEMPLATE void duplicate (ARRAY& to, ARRAYCR from) {
	to.duplicate(from);
}

TEMPLATE void free (ARRAY& a) {
	a.free();
}

TEMPLATE bool sharing (ARRAYCR a, ARRAYCR b) {
	return a.sharing(b);
}

/**************************************************************************/
/**********        Array2Cursor                            ****************/
/**************************************************************************/

TEMPLATE class Array2Cursor { 
	protected:
		ARRAY* myvector;
		Index myindex;
	public:

		/* queries */
		bool isvalid() const { return myindex < myvector->count(); } 
		bool isnextvalid() const { return myindex+1 < myvector->count(); } 
		DataCR item() const  { return myvector->at(myindex); }
		Data& item()   { return myvector->at(myindex); }
		Index index() const  { return myindex; }
		
		/* use */
		void reset (ARRAYCR thevec) { myvector=(ARRAY*)(&thevec);  myindex=0; }
		void assign (CURSORCR c) { myvector=c.myvector;  myindex=c.myindex; }

		Array2Cursor (ARRAYCR thevec) { reset(thevec); }
		Array2Cursor (CURSORCR thecursor) { assign(thecursor); }
		Array2Cursor () { myindex=0; }

		void operator= (CURSORCR thecursor) { assign(thecursor); }

		void next() { ++myindex; }
		void operator++() { next(); }

		const CURSOR* clone() const { return new CURSOR (myindex); }
};






/************************************************************************************************/
/************  	IO operations      **************************************************************/
/************************************************************************************************/


/* 
	The output format should contain at least one char, which is the item-delimiter.
	If the format is preceded by 'P', it says that the output will be surrounded
		by parenthesis.
	If the format is preceded by '*', it will be skipped.
	examples: (the vector  contains a,b,c and d)
		"P "           ==> [4: a b c d]
		"P,"           ==> [4: a,b,c,d]
		" "  (or "* ") ==> a b c d
		","  (or "*,") ==> a,b,c,d
*/
TEMPLATE void write (ostream& out, ARRAYCR thevec, Format format=" ") {
	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]
		format = format.next();
		out << '[';
		out << thevec.len() << ": ";
		write_vector (out,thevec, str(format[0]),format.next() );
		out << ']';
	}
	else {                          // Simple format: a1 a2 ... an
		if (format[0]=='*' || format[0]=='L')  format=format.next();
		write_vector (out,thevec, str(format[0]),format.next() );
	}
}

//TEMPLATE void write (ostream& out, ARRAYCR thevec) { write (out,thevec,""); }

TEMPLATE status read (istream& in, ARRAYR thevec, Format format="P") {
	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]
		format = format.next();
		DOr(testchar(in,'['));
		Size thelen=0;
		DOr(read(in,thelen));
		DOr(testchar(in,':'));
		DOr(thevec.realloc(thelen));
		return read_vector (in,thevec, /*separator*/format[0], /*stopper*/']', format.next() );
	}
	else if (format[0]=='L')    {   // line format: read elements until end-of-line
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




/**********************************************************************************************/
/**************************           find                  ***********************************/
/**********************************************************************************************/

/* find:
	This function searches the vector for the first occurence of the item.
	It searches from 'from' to 'to'-1, and returns the result in theresult.
	In order to use this function, the function 'compare' must be defined
		for the type Data (it is already defined for the basic C types,
		in the file "cpplib/typedefs.h").
*/



TEMPLATE status find (Data theitem, ARRAYCR thevec, Index from, Index to,
					  Index& theresult) 
{
	sort2 (from,to);
	if ( to > thevec.size() ) return ERANGE;

	for (Index i=from;  i<to;  ++i)
		if ( compare (thevec[i],theitem) == 0) {
			theresult=i;
			return OK;
		}
	return ENOTFOUND;
}	

TEMPLATE status find (Data theitem, ARRAYCR thevec, Index from, Index& theresult) 
	{ return find (theitem,thevec, from,thevec.size(), theresult); }

TEMPLATE status find (Data theitem, ARRAYCR thevec, Index& theresult) 
	{ return find (theitem,thevec, 0u,thevec.size(), theresult); }

TEMPLATE bool contains (ARRAYCR thevec, DataCR theitem) {
	LOOPVECTOR(;,thevec,i) 
		if ( identical(thevec[i],theitem) )  return true;
	return false;
}

/* findp / findcp ('c' for 'const') -- 
	returns a pointer to the item in the vector, or NULL if not found.
*/

TEMPLATE const Data* findcp (Data theitem, ARRAYCR thevec)
{
	Index theindex;
	if ( find(theitem,thevec,theindex) == ENOTFOUND)
		return NULL;
	else
		return &thevec[theindex];
}
TEMPLATE Data* findp (Data theitem, ARRAYR thevec)
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
TEMPLATE status find_and_remove (Data theitem, ARRAYR thevec) 
{
	Index theindex;
	DOr (find (theitem, thevec, theindex));
	return thevec.removeat(theindex);
}


#undef TEMPLATE
#undef ARRAY
#undef ARRAYR
#undef ARRAYCR
#undef CURSOR 
#undef CURSORR
#undef CURSORCR
#endif

