/*
 * File cpplib/map.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/map.hxx
 */

/* map.hxx -- a Key-Data map */


/**************************************************************************
	A Map is used to access items by general keys.
	For example, you can access numbers by strings (like a 'string array').
	You can choose the implementation of the map to your favorite container-type.
	Here are a few examples of Map definitions (in all cases, the key is CStr and the data is int):
		MapAsVector1<CStr,int> map1;     
		MapType(Array2,CStr,int) map5;      // map as Array2
		MapAsVector1Sorted<CStr,int> map2;     
		MapAsSlist0<CStr,int> map3;     
		MapAsHashTable<CStr,int> map4;     

	Here is a simple usage example: 
	#include <cpplib/map.hxx>
	#include <cpplib/slist0.hxx>        // NOTE: YOU MUST INCLUDE THIS 
	void main (void) {
		MapType(SList0,CStr,int) mymap;
		// equivalent to Map < SList0< KeyData<CStr,int> > , CStr , int > 
		mymap.insert ("abc",5);
		mymap.insert ("xyz",6);
		if (mymap.contains("ABC")) ...      // false!
		if (mymap.contains("abc"))   
			cout << mymap.item("abc"));     // will write '5'
		mymap.remove ("abc");
	}

	Please, look at the file 'TestMaps.cpp' for a usage example!
**************************************************************************/



#ifndef __MAP_HXX
#define __MAP_HXX

#include <cpplib/pair.hxx>

#define TEMPLATE3 template <class Container>
#define TEMPLATE2 template <class Key, class Data>

#define MAP Map<Container>
#define MAPCR const MAP&

#define CURSOR MapCursor<Container>
#define CURSORCR const CURSOR&

#define MapType(Container,Key,Data)  Map < Container < KeyData<Key,Data> >  >
#define MapCursorType(Container,Key,Data)  MapCursor < Container < KeyData<Key,Data> >  >



/**************************************************************************/
/**********        Map                            ***********************/
/**************************************************************************/

TEMPLATE3  class MapCursor;


TEMPLATE3  class Map {
	protected:
		Container myinfo;
		Size mycount; 
	public:
		typedef typename Container::Data Keydata;
		typedef typename Keydata::Key Key;
		typedef typename Keydata::Data Data;
		//	::Data Data;
		//typedef typename Container::Data::Key Key;
		typedef CURSOR Cursor;
		/****** queries ***********/
		bool isempty() const { return mycount==0; }
		Size count() const { return mycount; }

		/***** construct and destruct *******/
		//Map(Size thesize):  mycount(0), myinfo(thesize)  {  }  		
		Map():  mycount(0), myinfo()  {  }  		
		Map(Size thesize): mycount(0), myinfo(thesize) {}
		void free() { myinfo.free(); mycount=0; }
		status resize (Size thenewsize) { return myinfo.resize(thenewsize); }
		~Map() { /* nothing is done automatically */ }

		Container& info() { return myinfo; }
		const Container& info() const { return myinfo; }

		/* insert */
		/* the function does NOT check if the key already exists! check it yourself. */
		status insert (KeyCR thekey, DataCR thedata) {
			KEYDATA thepair (thekey,thedata);
			DOr ( myinfo.append(thepair) );
			++mycount;
			return OK;
		}

		/* remove -- */
		/*	 Returns ENOTFOUND if the key is not in this map  */
		/*	 If there are more than one -- removes one of them (no error check). */
		status remove (KeyCR thekey) {
			KEYDATA thepair (thekey);
			DOr ( find_and_remove (thepair,myinfo) );
			--mycount;
			return OK;
		}

		/* itemp --  */
		/*	return a pointer to the item that corresponds to the key (or NULL if not found). */
		/*	 if there are more than one -- returns one of them (no error check). */
		const Data* itemp (KeyCR thekey) const {
			KEYDATA thepair (thekey);
			const KEYDATA* therealpairp = findcp (thepair,myinfo);
			if (therealpairp==NULL)  return NULL;
			else return &therealpairp->data();
		}
		Data* itemp (KeyCR thekey) {
			KEYDATA thepair (thekey);
			KEYDATA* therealpairp = findp (thepair,myinfo);
			if (therealpairp==NULL) return NULL;
			else return &therealpairp->data();
		}

		/* find */
		bool contains (KeyCR thekey) const {
			return itemp(thekey) != NULL;
		}

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
		void duplicate (MAPCR from) {
			::duplicate ( myinfo, from.myinfo );
			mycount = from.mycount;
		}
};


TEMPLATE3 void duplicate (MAP& to, MAPCR from) {
	to.duplicate(from);
}

TEMPLATE3 void free (MAP& m) {
	m.free();
}


/**************************************************************************/
/**********        MapCursor                        ***********************/
/**************************************************************************/

TEMPLATE3  class MapCursor {
	protected:
		typename Container::Cursor mycursor;
	public:
		typedef typename Container::Data Keydata;
		typedef typename Keydata::Key Key;
		typedef typename Keydata::Data Data;

		/* queries */
		bool isvalid() const	{ return mycursor.isvalid(); } 
		KEYDATACR keydata() const { return mycursor.item(); }
		DataCR data() const	{ return mycursor.item().data(); }
		KeyCR key() const	{ return mycursor.item().key(); }
		KEYDATA& keydata()	{ return mycursor.item(); }
		DataR data(){ return mycursor.item().data(); }
		KeyR key() 	{ return mycursor.item().key(); }

		
		/* use */
		void reset (MAP& themap) { mycursor.reset(themap.info()); }
		void assign (CURSORCR c) { mycursor.assign(c.mycursor); }
		MapCursor (MAP& themap) { reset(themap); }
		MapCursor (CURSORCR thecursor) { assign(thecursor); }
		MapCursor () { }
		void operator= (CURSORCR thecursor) { assign(thecursor); }

		void next() { mycursor.next(); }
		void operator++() { next(); }

		//const CURSOR* clone() const { return new CURSOR (mycursor);	}		
};




/**************************************************************************/
/**********        IO operations                    ***********************/
/**************************************************************************/


/*
	general IO operations
*/

TEMPLATE3 void write_map (ostream& out, MAPCR themap, CStr the_keydata_separator, CStr the_line_separator, 
					  Format keyformat, Format dataformat) {
	if (themap.isempty()) return;
	typename Container::Cursor i ( ((MAP&)themap).info() );
	write_keydata (out, i.item(), the_keydata_separator, keyformat, dataformat);
	if (the_line_separator[0]=='\n')  out<<the_line_separator;
	for (i.next(); i.isvalid(); i.next() ) {
		write_keydata (out, i.item(), the_keydata_separator, keyformat, dataformat);
		out << the_line_separator;
	}
}

TEMPLATE3 status read_map (istream& in, MAP& themap, 
					 char the_keydata_separator, char the_line_separator, char the_stopper,
					 Format keyformat, Format dataformat) {
	if (the_line_separator==' ' || the_line_separator=='\n')  the_line_separator='\0';
	if (testchar(in,the_stopper)==OK) return OK; 
	for (Index i=0;;++i) {
		KeyData <MAP::Key, MAP::Data> theinput;
		DOr ( read_keydata (in, theinput, the_keydata_separator, keyformat, dataformat ) );
		DOr ( themap.insert ( theinput.key(), theinput.data() ) );
		if (testchar(in,the_stopper)==OK) return OK; 
		DOr ( testchar(in,the_line_separator) );
	}
}



/*
	format IO operations

	The first letter of the format should be one of:
		T/P -- parenthesized table format (string1 value1\n string2 value2\n...)
			(format.next() is the seperator between string and value))
		t   -- not parenthesized, table format.
*/

TEMPLATE3 void write (ostream& out, MAPCR themap, Format format) { 
	char f0 = format[0]; format = format.next();
	if (f0=='T'||f0=='P'||f0=='t') {                // parenthesized table format;
		if (f0!='t')  out << "{" << endl;
		if (format[0]==' ')	write_map (out,themap,str(" "),str("\n"), format.next(), format.next());
		else				write_map (out,themap,str(": "),str("\n"), format.next(), format.next());
		if (f0!='t')  out << "}";
	}
	else {
		if (format[0]=='*')  format=format.next();
		write (out, themap.info(), format);
	}
}

TEMPLATE3 void write (ostream& out, MAPCR themap) { write (out,themap.info() ); }

TEMPLATE3 status read (istream& in, MAP& themap, Format format) { 
	char f0 = format[0]; format = format.next();
	if (f0=='P' || f0=='T' || f0=='t')  {  // parenthesize: format.next() is the table separator
		if (f0!='t')  DOr(testchar(in,'{'));
		if (format[0]==' ')	return read_map (in,themap, ' ','\n','}', format.next(), format.next() );
		else				return read_map (in,themap, ':','\n','}', format.next(), format.next() );
	}
	else {
		if (format[0]=='*')  format=format.next();
		return read (in,themap.info(),format);
	}
}

TEMPLATE3 status read (istream& in, MAP& themap) { return read (in,themap.info()); }


/********************************************************************************/
/******   Definitions of several useful map types             *******************/
/********************************************************************************/

template <class Data> class HashTable;
template <class _Key, class _Data> class MapAsHashTable:	public MapType(HashTable,_Key,_Data)  {
	public:
		MapAsHashTable(Size bucketcount): MapType(HashTable,Key,Data)(bucketcount) {}
		MapAsHashTable(): MapType(HashTable,Key,Data)() {}
		status resize (Size thenewsize) { return myinfo.resize (thenewsize); }
};

template <class Data> class Vector1;
template <class _Key, class _Data> class MapAsVector1:		public MapType(Vector1,_Key,_Data) {
		static KeyData <Key,Data> dummy;   // required to work over a strange BUG.
	public: 
		typedef MapCursorType(Vector1,Key,Data) Cursor;
		MapAsVector1(): MapType(Vector1,Key,Data) () { }
		MapAsVector1(Size thesize): MapType(Vector1,Key,Data)(thesize)  {}
		status resize (Size thenewsize) { return myinfo.resize(thenewsize); }
};

template <class Data> class Vector1Sorted;
template <class _Key, class _Data> class MapAsVector1Sorted:		public MapType(Vector1Sorted,_Key,_Data) {
		static KeyData <Key,Data> dummy;   // required to work over a strange BUG.
	public: 
		MapAsVector1Sorted(): MapType(Vector1Sorted,Key,Data) () { }
};




/****************************************************************************************/
/*************** test map  **************************************************************/
/****************************************************************************************/

template <class MapClass> void test_map (MapClass& mymap, CStr thetestname) {
	cout << thetestname << " TEST:" << endl;
	cout << "map size: " << sizeof mymap << endl;
	mymap.insert ("",0);
	mymap.insert ("a",1);
	mymap.insert ("ab",2);
	mymap.insert ("abc",3);
	mymap.insert ("abc",900);    // allows a double key! 
	mymap.insert ("abcd",4);
	mymap.insert ("abcde",5);
	mymap.insert ("abcdef",6);
	mymap.insert ("abcdefg",7);

	/* writing the contents of the map in several formats */
	writeln (cout,mymap,Format("t "));
	writeln (cout,mymap,Format("T:"));         

	/* finding elements */
	if (mymap.contains("abc"))
		cout << "The number of abc is " << mymap.item("abc") << endl;
	else 
		cout << "abc is not contained in the map" << endl;

	if (mymap.contains("ABC"))
		cout << "The number of ABC is " << mymap.item("ABC") << endl;
	else 
		cout << "ABC is not contained in the map" << endl;

	/* removing elements */
	mymap.remove ("abcdef");
	mymap.remove ("abc");
	writeln (cout,mymap,Format("T "));  
	cout << endl;
}




#undef TEMPLATE2
#undef TEMPLATE3
#undef MAP
#undef MAPCR 
#undef CURSOR 
#undef CURSORCR

#endif
