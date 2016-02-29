/*
 * File cpplib/map1.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/map1.hxx
 */

/* map1.hxx -- a Key-Data map implemented as a sorted vector (minimum space). */
/*
	see the file map.hxx for explanation about maps.
*/


#ifndef __MAP1_HXX
#define __MAP1_HXX

#include <cpplib/pair.hxx>
#include <cpplib/vector1.hxx>

#define TEMPLATE2 template <class Key, class Data>
#define MAP Map1<Key,Data>
#define MAPR MAP&
#define MAPCR const MAP&
#define VECTOR Vector1Sorted < KeyData<Key,Data> >
#define VECTORR VECTOR&
#define VECTORCR const VECTOR&
#define CURSOR Map1Cursor<Key,Data>
#define CURSORR CURSOR&
#define CURSORCR const CURSOR&



/**************************************************************************
	A Map1 is used to access items by general keys.
	It is like a general map (see the file map.hxx), with a minor change
		that makes it more space-efficient.
	The Map1 is implemented by a Vector1Sorted (that is, the pairs are sorted 
		by the key).
	Please, look at the file 'TestMaps.cpp' for a usage example!
**************************************************************************/



/**************************************************************************/
/**********        Map1                            ***********************/
/**************************************************************************/

TEMPLATE2  class Map1Cursor;


template <class Key_, class Data_>  class Map1 {
	public:
		typedef Data_ Data;
		typedef Key_ Key;
	protected:
		VECTOR myinfo;
	public:
		typedef KeyData<Key,Data> Keydata;
		typedef CURSOR Cursor;
		/****** queries ***********/
		Size count() const { return myinfo.size(); }
		bool isempty() const { return count()==0; }

		/***** construct and destruct *******/
		Map1(Size thesize):  myinfo(thesize)  {  }  		
		Map1(): myinfo()  {  }  		
		void free () { myinfo.free();  }
		~Map1() { /* nothing is done automatically */ }

		VECTOR& info() { return myinfo; }
		const VECTOR& info() const { return myinfo; }

		/* insert */
		/* the function does NOT check if the key already exists! check it yourself. */
		status insert (KeyCR thekey, DataCR thedata) {
			KEYDATA thepair (thekey,thedata);
			return myinfo.insert(thepair);
		}

		/* remove -- */
		/*	 Returns ENOTFOUND if the key is not in this map  */
		/*	 If there are more than one -- removes one of them (no error check). */
		status remove (KeyCR thekey) {
			KEYDATA thepair (thekey);
			return myinfo.remove(thepair);
		}

		status removeat (Index at) { return myinfo.removeat(at); }

		/* itemp --  */
		/*	return a pointer to the item that corresponds to the key (or NULL if not found). */
		/*	 if there are more than one -- returns one of them (no error check). */
		const Data* itemp (KeyCR thekey) const {
			KEYDATA thepair (thekey);
			const KEYDATA* therealpairp = myinfo.findp(thepair);
			if (therealpairp==NULL)  return NULL;
			else return &therealpairp->data();
		}
		Data* itemp (KeyCR thekey) {
			KEYDATA thepair (thekey);
			KEYDATA* therealpairp = myinfo.findp(thepair);
			if (therealpairp==NULL) return NULL;
			else return &therealpairp->data();
		}

		/* item -- returns a reference to the item with thekey.  */
		/* if thekey is not found -- an error is caused. */
		DataCR item (KeyCR thekey) const {
			const Data* thepointer = itemp(thekey);
			assert(thepointer!=NULL);
			return *thepointer;
		}
		DataR item (KeyCR thekey) {
			Data* thepointer = itemp(thekey);
			assert(thepointer!=NULL);
			return *thepointer;
		}

		/* item_ins -- returns a reference to the item with thekey.  */
		/* if thekey is not found -- a new pair is inserted: (thekey,thedata). */
		DataR item_ins (KeyCR thekey, DataCR thedata) { 
			Data* thepointer = itemp(thekey);
			if (thepointer==NULL)  {  // insert a new pair
				DOx ( insert(thekey,thedata) );
				return item (thekey);    // now it MUST exist...
			}
			else 
				return *thepointer;
		}

		/* indexing */
		const Keydata& operator[] (Index i) const { return myinfo[i]; }
		Keydata& operator[] (Index i) { return myinfo[i]; }


		/* find */
		bool contains (KeyCR thekey) const {
			return itemp(thekey) != NULL;
		}


		/* resize */
		status resize (Size the_new_size) {
			return myinfo.resize (the_new_size);
		}

#ifdef UNIX
		friend void write<> (ostream& out, MAPCR themap);
#else
		friend void write (ostream& out, MAPCR themap);
#endif
};


TEMPLATE2 void duplicate (MAP& to, MAPCR from) {
	duplicate ( to.info(), from.info() );		}

TEMPLATE2 void free (MAP& m) {
	free ( m.info() ); }


/**************************************************************************/
/**********        Map1Cursor                        **********************/
/**************************************************************************/

TEMPLATE2  class Map1Cursor {
	protected:
		Vector1Cursor < KEYDATA > mycursor;    // This can be replaced by an Index (==uint).
	public: 

		/* queries */
		bool isvalid() const { return mycursor.isvalid(); } 

		DataCR data() const 
			{ assert(isvalid()); return mycursor.item().data(); }
		Data& data() 
			{ assert(isvalid()); return mycursor.item().data(); }
		KeyCR key() const 
			{ assert(isvalid()); return mycursor.item().key(); }

		KEYDATACR keydata() const 
			{ assert(isvalid()); return mycursor.item(); }
		Index index() const 
			{ assert(isvalid()); return mycursor.index(); }
		
		/* use */
		void reset (MAPR themap) { mycursor.reset(themap.info()); }
		void assign (CURSORCR c) { mycursor.assign(c.mycursor); }
		Map1Cursor (MAPR themap) { reset(themap); }
		Map1Cursor (CURSORCR thecursor) { assign(thecursor); }
		Map1Cursor () { }
		void operator= (CURSORCR thecursor) { assign(thecursor); }

		void next() { mycursor.next(); }
		void operator++() { next(); }

};




/**************************************************************************/
/**********        IO operations                    ***********************/
/**************************************************************************/


/*
	general IO operations
*/

TEMPLATE2 void write_map (ostream& out, MAPCR themap, CStr the_keydata_separator, CStr the_line_separator, 
					  Format keyformat, Format dataformat) {
	Index i=0;
	write_keydata (out, themap[i], the_keydata_separator, keyformat, dataformat);
	for ( ++i; i<themap.count(); ++i ) {
		out << the_line_separator;
		write_keydata (out, themap[i], the_keydata_separator, keyformat, dataformat);
	}
}

TEMPLATE2 status read_map (istream& in, MAPR themap, 
					 char the_keydata_separator, char the_line_separator, char the_stopper,
					 Format keyformat, Format dataformat) {
	if (the_line_separator==' ' || the_line_separator=='\n')  the_line_separator='\0';
	if (testchar(in,the_stopper)==OK) return OK; 
	for (Index i=0;;++i) {
		KEYDATA theinput;
		DOr ( read_keydata (in, theinput, the_keydata_separator, keyformat, dataformat ) );
		DOr ( themap.insert ( theinput.key(), theinput.data() ) );
		if (testchar(in,the_stopper)==OK) return OK; 
		DOr ( testchar(in,the_line_separator) );
	}
}




/* 
	format IO operations
*/

TEMPLATE2 void write (ostream& out, MAPCR themap, Format format) { 
	if (format[0]=='T') {                // table format;
		format=format.next();
		out << "[" << endl;
		write_map (out,themap, str(": "), str("\n"), format, format);
		out << "]";
	}
	else {
		if (format[0]=='*')  format=format.next();
		write (out, themap.info(), format);
	}
}

TEMPLATE2 void write (ostream& out, MAPCR themap) { write (out,themap.myinfo); }

TEMPLATE2 status read (istream& in, MAPR themap, Format format) { 
	if (format[0]=='T') {                // table format;
		format=format.next();
		DOr (testchar(in,'['));
		return read_map(in,themap, ':', '\n', ']', format, format);
	}
	else {
		if (format[0]=='*')  format=format.next();
		return read (in,themap.info(),format);
	}
}

TEMPLATE2 status read (istream& in, MAPR themap) { return read (in,themap.info()); }



#undef TEMPLATE2
#undef MAP
#undef MAPR 
#undef MAPCR 
#undef CURSOR 
#undef CURSORR 
#undef CURSORCR

#endif
