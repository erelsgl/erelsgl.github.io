/*
 * File cpplib/range.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/range.hxx
 */

/* range.hxx -- a general range template, and an integer range */

/************
	This unit contains 2 types.  Range is used for general ranges, and RangeNum is used for numerical ranges.
	Here is a usage example for them: {
		Range<char> r ('a','z');
		cout << r.contains('b');   // true
		cout << r.contains('B');   // false
		cout << r.contains('a');   // true
		cout << r.contains('z');   // false

		RangeNum<int> r (1,5);
		cout << r.length();        // 4
	}

	In order to use Range with your own type, you should define the function
		'compare' for that type. 
	For example, if you want to use Range<String>, you should define the function:
				short compare (const String& a, const String& b);
		which will return a short <, =, or > 0 according to the result of the comparison.
*************/

#ifndef __RANGE_HXX
#define __RANGE_HXX

#define TEMPLATE template <class Data>
#define RANGE Range<Data>
#define RANGER Range<Data>&
#define RANGECR const Range<Data>&


TEMPLATE class Range {
	protected:
		Data mylow, myhigh;      /* low IS included in the range, but high IS NOT */

	public:

		/********* constructor / set *********/
		void set (Data thelow, Data thehigh) {
			if ( compare(thelow,thehigh) <= 0 ) {
				mylow=thelow;  myhigh=thehigh;
			}
			else {
				mylow=thehigh;  myhigh=thelow;
			}
		}

		void set (RANGECR r) { set(r.mylow, r.myhigh); }
		Range (Data thelow, Data thehigh) { set (thelow, thehigh);	}
		Range (RANGECR r) { set(r);	}
		Range () {}



		/******* queries **********/
		Data low() const { return mylow; }
		Data high() const { return myhigh; }
		bool isempty() const { return compare(mylow,myhigh)==0; }

		bool contains (Data d) const {
			// return low<=d && d<high
			return (compare(mylow,d)<=0 && compare(d,myhigh)<0);
		}

};


TEMPLATE void write (ostream& out, RANGECR therange, Format theformat) {
	out << "[";
	write (out, therange.low(), theformat);
	out << "..";
	write (out, therange.high(), theformat);
	out << "]";
}

TEMPLATE void read (istream& in, RANGE& therange, Format theformat) {
	Data low, high;
	DOr ( testchar(in,'[') );
	read (in, low, theformat);
	DOr ( testchar(in,'.') );  	DOr ( testchar(in,'.') );
	read (in, high, theformat);
	DOr ( testchar(in,']') );
	therange.set (low,high);
}

TEMPLATE void duplicate (RANGE& to, RANGECR from) { 
	to.set (from.low(),from.high()); }


TEMPLATE void free (RANGE& r) { }




TEMPLATE class RangeNum: public RANGE {           // range of numbers
	public:
		void set (Data thelength) { set (0,thelength); }
		RangeNum (Data thelength) { set (thelength); }
		Data length() const { return myhigh-mylow; }
};



#undef TEMPLATE
#endif

