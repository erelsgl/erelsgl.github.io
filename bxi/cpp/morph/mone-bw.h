/*
 * File morph/mone-bw.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mone-bw.h
 */

/* mone-bw.h -- baseword (with HeleqDiber) counter */

#ifndef __MONEBW_H
#define __MONEBW_H

#include <cpplib/wordcnt.h>
#include <cpplib/sikui.h>
#include <morph/morphinf.h>

/*        ***** USAGE EXAMPLE ***** (OUTDATED!)
// inserting and counting:
	MoneBaseword yourcounter;
	yourcounter.insert ("&WLH",ECEM);
	yourcounter.insert ("&WLH",TOAR);
	yourcounter.insert ("&WLH",ECEM);
	yourcounter.insert ("&LH",POAL);
	cout << yourcounter.count("&WLH",ECEM);       // prints 2
	cout << yourcounter.count("&WLH",TOAR);       // prints 1
	cout << yourcounter.count("&WLH",POAL);       // prints 0
	cout << yourcounter.count("&LH",POAL);       // prints 1
	cout << yourcounter.count("&LH",ECEM);       // prints 0
	cout << yourcounter.wordcount();        // prints 4 (4 basewords total)
	cout << yourcounter.count();            // prints 3 (3 different basewords)

//saving the counter to a file:
	ofstream your_output_stream("file.txt");
	write (your_output_stream, yourcounter );
	yout_output_stream.close();
//restoring the counter from the file:
	ifstream your_input_stream("file.txt");
	read (your_input_stream, yourcounter );
	yout_input_stream.close();
*/



class MoneBaseword;
typedef const MoneBaseword& MoneBasewordCR;

class MoneBaseword {
	WordCounter mycounters [16];
	WordCounter& counter (HeleqDiber hd) { return mycounters[hd+8]; }
public:
	void zero_all() {
		for (Index i=0; i<16; ++i) 
			mycounters[i].zero_all();
	}
	status setcount (MorphInfoCR theinfo, Sikui thesikui=0) {
		return counter(theinfo.heleqdiber()).setcount (theinfo.baseword(), thesikui ); }
	status add (MorphInfoCR theinfo, Sikui thesikui=SIKUI1) {
		return counter(theinfo.heleqdiber()).insert (theinfo.baseword(), thesikui ); }
	Count count (MorphInfoCR theinfo) {
		return counter(theinfo.heleqdiber()).count (theinfo.baseword() ); }
	bool isempty() const {
		for (Index i=0; i<16; ++i) 
			if (!mycounters[i].isempty()) return false;
		return true;
	}

	friend void write (ostream& out, MoneBasewordCR theinfo);
	friend status read (istream& in, MoneBaseword& theinfo);
	friend class MoneBasewordCursor;

	typedef MoneBasewordCursor Cursor;

};



void write (ostream& out, MoneBasewordCR theinfo) {
//	out << "{" << endl;
	for (Index i=0; i<16; ++i) {
		write(out, theinfo.mycounters[i], Format("T")); 
//		out<<".";
	}
//	out << "}" << endl;
}

status read (istream& in, MoneBaseword& theinfo) {
//	DOr(testchar(in,'{'));
	for (Index i=0; i<16; ++i)
		DOr(read (in,theinfo.mycounters[i]));
//		read_trie (in,theinfo.mycounters[i],':','\n','.',Format(""));
//	DOr(testchar(in,'}'));
	return OK;
}








class MoneBasewordCursor {
protected:
	MoneBaseword* my_mone;
	int my_counter_index;
	WordCounter::Cursor my_counters_cursor;

	void go_to_next_valid () {
		for (;;) {
			if (my_counters_cursor.isvalid()) break;    // found valid
			++my_counter_index;
			if (my_counter_index>=16) break;            // didn't find valid
			my_counters_cursor.reset(my_mone->mycounters[my_counter_index]);
		}
	}

public:

	/* queries */
	CStr data() const { return my_counters_cursor.data(); }
	Count& count() { return my_counters_cursor.count(); }
	const Count& count() const { return my_counters_cursor.count(); }
	bool isvalid() const { 
		return my_counter_index < 16;
	}


	/* use */
	void reset (MoneBaseword& the_mone) {
		my_mone = &the_mone;
		my_counter_index = 0;
		my_counters_cursor.reset(my_mone->mycounters[my_counter_index]);
		go_to_next_valid();
	}

	MoneBasewordCursor (MoneBaseword& the_mone) { reset(the_mone);  }

	void next() { 
		my_counters_cursor.next();
		go_to_next_valid();
	}

	void operator++() { next(); }
};


#endif
