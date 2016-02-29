/*
 * File cpplib/mapc.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/mapc.hxx
 */

/* mapc.hxx -- a Key-Container Map */

#ifndef __MAPC_HXX
#define __MAPC_HXX

/**************************************************************************
	This file defines a template called "MapOfContainer".
	It is like a usual map (see "map.hxx"), just that the "Data" type is a container.
	For example, you can use it to define a map that maps a word to all it's possible 
		interpretations (like a dictionary).
	Here are a few examples of Map definitions (in all cases, the key is CStr and the data is int):
		MapOfContainerType(Map1,SList0,CStr,int) map1;      // Maps a CStr to a SList0 of ints
		Trie0OfContainerType(SList0,int) map2;                 // Maps a CStr to a SList0 of ints, using a trie.

	Here is a simple usage example: 

  	#include <cpplib/map1.hxx>
	#include <cpplib/slist0.hxx>
	#include <cpplib/mapc.hxx>

	typedef SList0<int> IntList;
	typedef Map1<CStr,IntList> IntListMap;

	void main (void) {
		MapOfContainer<IntListMap,IntList,CStr,int> mymap;
		// equivalent to: MapOfContainerType(Map1,SList0,CStr,int) mymap;
		mymap.insert ("abc",5);
		mymap.insert ("xyz",6);
		mymap.insert ("abc",7);
		cout << mymap.count("abc");     // 2
		cout << mymap.count("xyz");     // 1
		cout << mymap.count("ABC");     // 0
	}
**************************************************************************/


// The 'MapClass' must map keys of type 'Key' to items of type 'Container'.
// The 'Container' must contain items of type 'Data'.
//#define TEMPLATE4  template <class Map, class Container, class Key, class Data>
#define TEMPLATE4  template <class MapClass>

//#define MAPCONT MapOfContainer<Map,Container,Key,Data>
#define MAPCONT MapOfContainer<MapClass>
#define MAPCONTR MAPCONT&
#define MAPCONTCR const MAPCONT&


//#define MapOfContainerType(Map,Container,Key,Data)  \
//	MapOfContainer <  Map < Key , Container<Data> >  ,  Container<Data>  ,  Key  ,  Data  >
#define MapOfContainerType(Map,Container,Key,Data)  MapOfContainer <  Map < Key , Container<Data> > >

//#define Trie0OfContainerType(Container,Data)  \
//	MapOfContainer <  Trie0 < Container<Data> >  ,  Container<Data>  ,  CStr  ,  Data  >
#define Trie0OfContainerType(Container,Data)  MapOfContainer <  Trie0 < Container<Data> >  >

//#define Trie1OfContainerType(Container,Data)  \
//	MapOfContainer <  Trie1 < Container<Data> >  ,  Container<Data>  ,  CStr  ,  Data  >
#define Trie1OfContainerType(Container,Data)  MapOfContainer <  Trie1 < Container<Data> >  >


/**************************************************************************/
/**********        MapOfContainer                   ***********************/
/**************************************************************************/

TEMPLATE4  class MapOfContainer {
	protected:
		MapClass mymap;                   // this should be a map that maps 'key' to 'Container<Data>'

	public:

		typedef typename MapClass::Key Key;
		typedef typename MapClass::Data Container;
		typedef typename Container::Data Data;

		/***** construct and destruct *******/
		MapOfContainer():  mymap()  {  }  		
		void free() { mymap.free(); }
		~MapOfContainer() { /* nothing is done automatically */ }

		/************ queries **************************/
		MapClass& info() { return mymap; }                                           // return the whole map
		const MapClass& info() const { return mymap; }      

		Container* infop (Key thekey) { return mymap.itemp(thekey); }           // return the container that matches a single key (or NULL if not found)
		const Container* infop (Key thekey) const { return mymap.itemp(thekey); }

		Size count (Key thekey) const {         // the size of the container of the-key
			const Container* containerp = infop(thekey);
			if (containerp==NULL)    return 0;
			else return containerp->count();
		}

		// insert the data to the appropriate container (does NOT check for duplicates!)
		status insert (Key thekey, Data thedata) {    
			Container* containerp = infop(thekey);
			if (containerp==NULL) {        // container does not exist yet
				Container inserted;
				DOr ( inserted.append (thedata) );
				DOr ( mymap.insert (thekey, inserted) );   //  insert a new container
				return OK;
			}
			else {                         // container already exists
				return containerp->append(thedata);
			}
		}

		status remove (Key thekey) {    
			return mymap.remove (thekey); 
		}
};


TEMPLATE4 void duplicate (MAPCONT& to, MAPCONTCR from) {
	duplicate ( to.info(), from.info() );				}

TEMPLATE4 void free (MAPCONT& m) {
	free ( m.info() );				}


/**************************************************************************/
/**********        MapOfContainerCursor             ***********************/
/**************************************************************************/

TEMPLATE4  class MapOfContainerCursor: public MapClass::Cursor {
	public:
		typedef typename MapClass::Key Key;
		typedef typename MapClass::Data Container;
		typedef typename Container::Data Data;
		typedef typename MapClass::Cursor super;

		/* queries */
		// Key key() const    // inherited from MapCursor
		const Container& container() const { return data(); }
		
		/* use */
		void reset (MAPCONTCR themap) { super::reset(themap.info()); }
		MapOfContainerCursor (MAPCONTCR themap) { reset(themap); }
		MapOfContainerCursor () { }
};





/**************************************************************************/
/**********        IO operations                    ***********************/
/**************************************************************************/

TEMPLATE4 void write (ostream& out, MAPCONTCR themap, Format format) {
/*	if (format[0]=='T') {
		write (out,themap.info(),Format("T ="));
	} 
	else {
		if (format[0]=='*') format=format.next();*/
		write (out,themap.info(),format);
	//}
}

TEMPLATE4 void write (ostream& out, MAPCONTCR themap) { write (out,themap.info()); }

TEMPLATE4 status read (istream& in, MAPCONTR themap, Format format) { 
	return read (in, themap.info(), format);
}

TEMPLATE4 status read (istream& in, MAPCONTR themap) { return read (in,themap.info()); }

TEMPLATE4 void save (ostream& out, MAPCONTCR themap) { 
	save (out, themap.info()); 
}

TEMPLATE4 void load (ostream& out, MAPCONTR themap) { 
	load (in, themap.info());
}




#undef TEMPLATE4
#undef MAPCONT 
#undef MAPCONTR 
#undef MAPCONTCR

#endif
