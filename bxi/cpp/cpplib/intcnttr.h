/*
 * File cpplib/intcnttr.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/intcnttr.h
 */

/* intcnttr.h -- int counter trie */



#ifndef __INTCNTTR_H

#define __INTCNTTR_H



#include <cpplib/trie0.hxx>

#include <cpplib/countc.hxx>

#include <cpplib/mapc.hxx>

#include <cpplib/map1.hxx>



/*        ***** USAGE EXAMPLE *****

// inserting and counting:

	IntCounterTrie yourcounter;

	yourcounter.insert ("ABC",10);

	yourcounter.insert ("ABC",10);

	yourcounter.insert ("ABC",20);

	yourcounter.insert ("abc",20);

	yourcounter.insert ("abc",20);

	yourcounter.insert ("abc",30);

	cout << yourcounter.count("ABC",10);       // prints 2

	cout << yourcounter.count("ABC",20);       // prints 1

	cout << yourcounter.count("ABC",30);       // prints 0

	cout << yourcounter.count("abc",10);       // prints 0

	cout << yourcounter.count("abc",20);       // prints 2

	cout << yourcounter.count("abc",30);       // prints 1



//counting words in a file:

	ifstream your_input_stream ("words.txt");

	yourcounter.count_words_in_file (your_input_stream);



//saving the counter to a file:

	ofstream your_output_stream("file.txt");

	write (your_output_stream, yourcounter );

	yout_output_stream.close();

//restoring the counter from the file:

	ifstream your_input_stream("file.txt");

	read (your_input_stream, yourcounter );

	yout_input_stream.close();

*/



typedef CounterVector1Sorted<uint> IntCounter;

typedef const IntCounter& IntCounterCR;



extern IntCounter the_empty_int_counter;



class IntCounterTrie: public Trie0OfContainerType(CounterVector1Sorted,uint) {

public:

	uint count (CStr theword, uint thenum) {

		CounterVector1Sorted<uint>* p = infop(theword);

		if (p==NULL) return 0;

		else return p->count(thenum);

	}

	IntCounter& int_counter (CStr theword) {

		IntCounter* the_int_counter = infop(theword);

		if (the_int_counter!=NULL)  return *the_int_counter;

		else return the_empty_int_counter;

	}

};

typedef const IntCounterTrie& IntCounterTrieCR;





inline void write  (ostream& out, IntCounterTrieCR theinfo) { write(out,theinfo,Format("T:L P")); }



inline status read  (istream& in, IntCounterTrie& theinfo) { return read (in,theinfo,Format("T:L P"));  }







#endif

