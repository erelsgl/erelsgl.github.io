/*
 * File cpplib/pair.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/pair.hxx
 */

/* pair.hxx -- 2 general pair templates */

/* 
	This unit defines 2 classes, both of which are used to associate 2 variables in 1 variable.
		KeyData -- associates variables of 2 different types: a Key and a Data.
		Pair -- associates 2 variables of the same type (Data)
*/

#ifndef __PAIR_HXX
#define __PAIR_HXX

#include <cpplib/klli1.h>


#define PAIR Pair<Data>
#define PAIRCR const Pair<Data>&
#define TEMPLATE template<class Data>

#define KEYDATA KeyData<Key,Data>
#define KEYDATACR const KeyData<Key,Data>&
#define TEMPLATE2 template<class Key, class Data>


/**********************************************************************************/
/*************			KeyData                                  ******************/
/**********************************************************************************/

template<class Key_, class Data_> class KeyData {
	public:
		typedef Key_ Key;
		typedef Data_ Data;
	protected:
		Key mykey;
		Data mydata;      
	public:

		/********* constructor / set *********/
		void set (KeyCR thekey, DataCR thedata) {
			mykey=thekey;  mydata=thedata;
		}
		void set (KeyCR thekey) {
			mykey=thekey;  
		}
		void set (KEYDATACR r) { set(r.mykey, r.mydata); }
		KeyData (Key thekey, Data thedata): mykey(thekey), mydata(thedata){}
		KeyData (Key thekey): mykey(thekey) /*, mydata() */{}
		KeyData (KEYDATACR r) { set(r);	}
		KeyData () /*: mykey(), mydata()*/ {}

		/******* queries **********/
		KeyCR key() const { return mykey; }
		DataCR data() const { return mydata; }
		KeyR key() { return mykey; }
		DataR data() { return mydata; }

		bool operator== (uint ZERO) const { return mykey==0 && mydata==0; }
		void operator=  (uint ZERO) { mykey=0; mydata=0; }
};



// the compare function compares the keys and the ignores the data
TEMPLATE2  short compare (KEYDATACR a, KEYDATACR b) {
	return compare (a.key(),b.key());
}

TEMPLATE2  bool identical (KEYDATACR a, KEYDATACR b) {
	return identical (a.key(),b.key());
}

TEMPLATE2 int hashvalue (KEYDATACR a) {
	return hashvalue (a.key());
}



/************************************************************************************************/
/************  	IO operations      **************************************************************/
/************************************************************************************************/

TEMPLATE2 void write_keydata (ostream& out, KEYDATACR a, CStr theseparator, Format keyformat, Format dataformat) {
	write_pair (out,a.key(),theseparator,a.data(),keyformat,dataformat);
}

TEMPLATE2 status read_keydata (istream& in, KEYDATA& a, char theseparator, Format keyformat, Format dataformat) {
	return read_pair (in,a.key(),theseparator,a.data(),keyformat,dataformat);
}



TEMPLATE2 void write (ostream& out, KEYDATACR a, Format format="") {
	if (format[0]=='P')  {  // Parenthesized format
		out << '<';   write_keydata(out,a,str(":"),format.next(),format.next());   out << '>';
	}
	else if (format[0]==':') {
		write_keydata(out,a,str(": "),format.next(),format.next());
	}
	else if (format[0]=='=') {
		write_keydata(out,a,str("="),format.next(),format.next());
	}
	else {
		write_keydata(out,a,str(" "),format.next(),format.next());
	}
}


TEMPLATE2 status read (istream& in, KEYDATA& a, Format format="") {
	if (format[0]=='P')  {  // Parenthesized format
		TESTCHAR(in,'<');  
		DOr( read_keydata(in,a,':',format.next(),format.next()) );   
		TESTCHAR(in,'>');  
		return OK;
	}
	else if (format[0]==':') {
		return read_keydata(in,a,':',format.next(),format.next());   
	}
	else if (format[0]=='=') {
		return read_keydata(in,a,'=',format.next(),format.next());
	}
	else {
		return read_keydata(in,a,' ',format.next(),format.next());
	}
}


TEMPLATE2 void duplicate (KEYDATA& to, KEYDATACR from) {
	duplicate ( to.key(), from.key() );
	duplicate ( to.data(), from.data() );
}

TEMPLATE2 void free (KEYDATA& kd) {
	free ( kd.key() );
	free ( kd.data() );
}



/**********************************************************************************/
/*************			Pair                                     ******************/
/**********************************************************************************/

#define KD KeyData<_Data,_Data>

template<class _Data> class Pair: public KD {
	public:
		Pair (Data thex, Data they): KD(thex,they) {}
		Pair (uint ZERO)  { mykey=0; mydata=0;}
		Pair (): KD() {}
		bool operator== (uint ZERO) const { return mykey==0 && mydata==0; }
		void operator=  (uint ZERO) { mykey=0; mydata=0; }
		/******* queries **********/
		DataCR x() const { return mykey; }
		DataCR y() const { return mydata; }
		DataR x() { return mykey; }
		DataR y() { return mydata; }
};

// the compare function compares the pairs by the lexicographical order
//		(x is more important than y)
TEMPLATE  short compare (PAIRCR a, PAIRCR b) {
	short cx = compare (a.x(),b.x());
	if ( cx < 0 ) return -1;
	if ( cx > 0 )    return 1;
	return compare (a.y(),b.y());
}

TEMPLATE2  bool identical (PAIRCR a, PAIRCR b) {
	return identical (a.x(),b.x()) && identical (a.data(),b.data());
}

TEMPLATE int hashvalue (PAIRCR a) {
	return hashvalue (a.x()) + hashvalue(a.y());
}


TEMPLATE void write (ostream& out, PAIRCR a, Format format="") {
	if (format[0]=='P')  {  // Parenthesized format
		out << '(';   write_keydata(out,a,str(","),format.next(),format.next());   out << ')';
	}
	else  {
		write_keydata(out,a,str(" "),format.next(),format.next());
	}
}

TEMPLATE status read (istream& in, PAIR& a, Format format="") {
	if (format[0]=='P')  {  // Parenthesized format
		TESTCHAR(in,'(');  
		DOr( read_keydata(in,a,',',format.next(),format.next()) );   
		TESTCHAR(in,')');  
		return OK;
	}
	else {
		return read_keydata(in,a,' ',format.next(),format.next());
	}
}



#undef PAIR
#undef PAIRCR
#undef TEMPLATE 
#undef TEMPLATE2

#endif

