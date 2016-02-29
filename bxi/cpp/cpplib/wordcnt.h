/*
 * File cpplib/wordcnt.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/wordcnt.h
 */

/* wordcnt.h -- word counter */

#ifndef __WORDCNT_H
#define __WORDCNT_H

#include <cpplib/trie0.hxx>
#include <cpplib/countc.hxx>

/*        ***** USAGE EXAMPLE *****
// inserting and counting:
	WordCounter yourcounter;
	yourcounter.insert ("ABC");
	yourcounter.insert ("abc");
	yourcounter.insert ("ABC");
	cout << yourcounter.count("ABC");       // prints 2
	cout << yourcounter.count("abc");       // prints 1
	cout << yourcounter.count("Abc");       // prints 0
	cout << yourcounter.wordcount();        // prints 3 (3 words total)
	cout << yourcounter.count();            // prints 2 (2 different words)

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




class WordCounter: public CounterTrie0 {
	private:
		Size my_wordcount;
	public:
		typedef CounterTrie0Cursor Cursor;
		WordCounter(int ZERO=0): CounterTrie0() { my_wordcount=0; }
		status insert (CStr thedata, uint howmany=1) { 
			if ( (the_status=CounterTrie0::insert(thedata,howmany)) == OK ) { my_wordcount+=howmany; return OK; }
			else return the_status;
		}
		status remove_one (CStr thedata, uint howmany=1) { 
			if ( (the_status=CounterTrie0::remove_one(thedata,howmany)) == OK ) { my_wordcount-=howmany; return OK; }
			else return the_status;
		}
		status remove (CStr thedata) { 
			uint howmany = count(thedata);
			if ( (the_status=CounterTrie0::remove_one(thedata,howmany)) == OK ) { my_wordcount-=howmany; return OK; }
			else return the_status;
		}
		status count_words_in_file  (istream& in, Index from_wordnum=0, Index to_wordnum=0x7ffffff, CStr the_word_that_will_stop_the_count="\0x26");
		Count wordcount() const { return my_wordcount; }
		void update_wordcount() {         
			my_wordcount=0;
			for (Cursor c(*this); c.isvalid(); ++c) 
				my_wordcount += c.count();
		}
		void zero_all () {                      // zero all counts
			//for (Cursor c(*this); c.isvalid(); ++c) 
			//	c.count() = 0;
			CounterTrie0::zero_all();
			my_wordcount = 0;
		}

		void operator= (int ZERO) { zero_all(); }
		bool operator== (int ZERO) const { return my_wordcount==0; }
};

typedef const WordCounter& WordCounterCR;
typedef CounterTrie0Cursor WordCounterCursor;


inline void write  (ostream& out, WordCounterCR theinfo, Format format) { 
	if (format[0]=='t') write(out,(CounterTrie0)theinfo,Format("t "));
	else if (format[0]=='L') write(out,(CounterTrie0)theinfo,Format("L "));
	else write(out,(CounterTrie0)theinfo,Format("T "));
}
inline status read  (istream& in, WordCounter& theinfo, Format format) {
	if (format[0]=='t') { DOr(read (in,(CounterTrie0&)theinfo,Format("t "))); }
	else if (format[0]=='L') { DOr(read (in,(CounterTrie0&)theinfo,Format("L "))); }
	else { DOr(read (in,(CounterTrie0&)theinfo,Format("T "))); }
	theinfo.update_wordcount(); return OK; 
}

inline void write  (ostream& out, WordCounterCR theinfo) { write(out,theinfo,Format("T  ")); }
inline status read  (istream& in, WordCounter& theinfo) { return read (in,theinfo,Format("T  "));  }



#endif
