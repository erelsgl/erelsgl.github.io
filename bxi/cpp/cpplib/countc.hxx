/*
 * File cpplib/countc.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/countc.hxx
 */

/* COUNTC.hxx -- a Counter-Container */

#ifndef __COUNTC_HXX
#define __COUNTC_HXX

#include <cpplib/klli1.h>

/**************************************************************************
	This file defines a template called "CounterContainer".
	It is like a usual container, just that each item can occur more than once, and the
		container counts how much times each item appears.
	Here are a few examples of CounterContainer definitions (in all cases, the key is CStr and the data is int):
		CounterContainerType(Map1,CStr) c1;      // counts strings
		CounterVector1Sorted<int> c2;          // counts ints

	Here is a simple usage example: 

  	#include <cpplib/countc.hxx>
	#include <cpplib/map.hxx>
	#include <cpplib/vector1.hxx>
	
	void main (void) {
		CounterVector1<CStr> myvec;
		// equivalent to:  CounterContainerType(MapAsVector1,CStr) myvec;
		myvec.insert ("abc");
		myvec.insert ("xyz");
		myvec.insert ("abc");
		cout << myvec.count("abc");     // 2
		cout << myvec.count("xyz");     // 1
		cout << myvec.count("ABC");     // 0
	}
**************************************************************************/


typedef uint Count;


// The 'MapClass' must map keys of type 'Data' to items of type 'Count' (or uint).
#define TEMPLATE2  template <class MapClass>
#define TEMPLATE2C  template <class MapClass>

#define TEMPLATE1  template <class Data>

#define COUNTCONT CounterContainer<MapClass>
#define COUNTCONTCR const COUNTCONT&

#define CURSOR  CounterContainerCursor<MapClass>
#define CURSORCR const CURSOR&


#define CounterContainerType(Map,Data)  CounterContainer <  Map < Data , Count >  >
#define CounterContainerCursorType(Map,Data)  CounterContainerCursor <  Map < Data , Count >   >
#define CounterTrie0  CounterContainer <  Trie0 < Count >  >
#define CounterTrie0Cursor  CounterContainerCursor <  Trie0 < Count >  >


/**************************************************************************/
/**********        CounterContainer                   ***********************/
/**************************************************************************/

TEMPLATE2C  class CounterContainerCursor;


TEMPLATE2  class CounterContainer {
	protected:
		MapClass mymap;                   // this should be a map that maps 'Data' to 'Count' (uint).

	public:
		typedef typename MapClass::Key Data;
		typedef CURSOR Cursor;

		/***** construct and destruct *******/
		CounterContainer():  mymap()  {  }  		
		CounterContainer(Size thesize):  mymap(thesize)  {  }
		status resize(Size thenewsize) { return mymap.resize(thenewsize); }
		void free () { mymap.free(); }
		~CounterContainer() { /* nothing is done automatically */ }

		/************ queries **************************/
		bool isempty() const { return mymap.isempty(); }
		MapClass& info() { return mymap; }                                           // return the whole map
		const MapClass& info() const { return mymap; } 
		
		Count count (DataCR thedata) const {         // the count associated with the-key
			const Count* countp = mymap.itemp(thedata);
			if (countp==NULL)    return 0;
			else return *countp;
		}

		bool contains (DataCR thedata) const {
			return mymap.contains(thedata); }

		Count  count() const { return mymap.count(); }

		status setcount (DataCR thedata, uint thecount=0) {    // set the count of thedata to 'thecount'.
			Count* countp = mymap.itemp(thedata);
			if (countp==NULL)         // item does not exist yet
				return mymap.insert(thedata,thecount);   //  insert a new counter
			else {                         // counter already exists
				(*countp)=thecount;
				return OK;
			}
		}

		status insert (DataCR thedata, uint howmany=1) {    // insert 'howmany' occurances of thedata.
			Count* countp = mymap.itemp(thedata);
			if (countp==NULL)         // item does not exist yet
				return mymap.insert(thedata,howmany);   //  insert a new counter
			else {                         // counter already exists
				(*countp)+=howmany;
				return OK;
			}
		}

		status append (DataCR thedata) { return insert(thedata); }

		status add (DataCR thedata, uint howmany) { return insert(thedata,howmany); }

		status remove_one (DataCR thedata, uint howmany=1) {    // remove 'howmany' occurances of thedata
			Count* countp = mymap.itemp(thedata);
			if (countp==NULL)         // item does not exist yet
				return OK;
			else {                         // counter already exists
				if (*countp >= howmany)
					(*countp) -= howmany;
				if (*countp == 0) 
					return mymap.remove(thedata);
				else 
					return OK;
			}
		}

		status remove (DataCR thedata) {    // remove all occurances of thedata
			return mymap.remove(thedata);
		}

		void zero_all () {                      // zero all counts
			for (Cursor c(*this); c.isvalid(); ++c) 
				c.count() = 0;
		}

		bool operator==(int ZERO) const { return isempty(); }
		void operator= (int ZERO) { free(); }

		void duplicate (COUNTCONTCR from) {
			::duplicate(mymap,from.mymap);
		}

};


TEMPLATE2 void duplicate (COUNTCONT& to, COUNTCONTCR from) {
	to.duplicate(from);
}

TEMPLATE2 void free (COUNTCONT& c) {
	c.free(); 
}



/**************************************************************************/
/**********        CounterContainerCursor             ***********************/
/**************************************************************************/

TEMPLATE2C  class CounterContainerCursor {
	protected:
		typename MapClass::Cursor mycursor;
	public:
		typedef typename MapClass::Key Data;

		/* queries */
		DataCR data() const { return mycursor.key(); }
		Count& count() { return mycursor.data(); }
		const Count& count() const { return mycursor.data(); }
		bool isvalid() const { return mycursor.isvalid(); }

		/* use */
		void reset (COUNTCONT& thecont) { mycursor.reset(thecont.info()); }
		CounterContainerCursor (COUNTCONT& thecont): mycursor(thecont.info()) {  }
		CounterContainerCursor () { }
		void next() { 
			for(;;) {
				mycursor.next();
				if (!isvalid()) break;
				if (!(data()==0))  break;
			}
		}

		void operator++() { next(); }
};



/**************************************************************************/
/**********        IO operations                    ***********************/
/**************************************************************************/

/*
	The first letter of the format should be one of:
		T/P -- parenthesized table format (item1 count1\n item2 count2\n...)
			(format.next() is the seperator between item and count))
		t   -- not parenthesized, table format.
*/

TEMPLATE2 void write (ostream& out, COUNTCONTCR themap, Format format) {
	write (out,themap.info(),format); }

TEMPLATE2 void write (ostream& out, COUNTCONTCR themap) { write (out,themap.info() ); }

TEMPLATE2 status read (istream& in, COUNTCONT& themap, Format format) { 
	return read (in, themap.info(), format); }

TEMPLATE2 status read (istream& in, COUNTCONT& themap) { return read (in,themap.info() ); }


/********************************************************************************/
/******   Definitions of several useful counter types         *******************/
/********************************************************************************/

template <class Key, class Data> class MapAsHashTable;
TEMPLATE1 class CounterHashTable:	public CounterContainerType(MapAsHashTable,Data) {
	public:
		status resize (Size thenewsize) { return mymap.resize (thenewsize); }
};

template<class Key, class Data> class MapAsVector1;
template<class _Data> class CounterVector1:		public CounterContainerType(MapAsVector1,_Data) {
	public: 
		typedef CounterContainerCursorType(MapAsVector1,Data) Cursor;
		CounterVector1 (Size the_size=0): CounterContainerType(MapAsVector1,Data)(the_size)  { }
		status resize (Size thenewsize) { return mymap.resize(thenewsize); }
		bool operator==(int ZERO) const { return isempty(); }
		void operator= (int ZERO) { free(); }
};

template<class Key, class Data> class Map1;
template<class _Data> class CounterVector1Sorted:		public CounterContainerType(Map1,_Data) {
	public: 
		CounterVector1Sorted (Size thesize=0): CounterContainerType(Map1,Data)(thesize) {}
		status resize (Size thenewsize) { return mymap.resize(thenewsize); }
};







#undef TEMPLATE2
#undef TEMPLATE1
#undef COUNTCONT 
#undef COUNTCONTCR
#undef CURSOR
#undef CURSORCR

#endif
