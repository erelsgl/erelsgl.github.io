/*
 * File cpplib/wordcnt2.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/wordcnt2.h
 */

/* wordcnt2.h -- word-pair counter */

#ifndef __WORDCNT2_H
#define __WORDCNT2_H

#include <cpplib/wordcnt.h>
#include <cpplib/stringt.h>


/*        ***** USAGE EXAMPLE *****
	(see "wordcnt.h")
*/




class WordPairCounter: public WordCounter {
	public:
		#define concat2(a,b) concat(a,' ',b).str
		#define concat3(a,b,c) concat(concat(a,' ',b).str,' ',c).str
		#define concat4(a,b,c,d) concat(concat(a,' ',b).str,' ',concat(c,' ',d).str).str
		Count count (CStr a, CStr b) { return WordCounter::count (concat2(a,b)); }
		status insert (CStr a, CStr b, uint howmany=1) { return WordCounter::insert ( concat2(a,b), howmany ); }
		status remove_one (CStr a, CStr b, uint howmany=1) { return WordCounter::remove_one ( concat2(a,b), howmany ); }
		status remove (CStr a, CStr b) { return WordCounter::remove ( concat2(a,b) ); }
		Count count (CStr a, CStr b, CStr c) { return WordCounter::count (concat3(a,b,c)); }
		status insert (CStr a, CStr b, CStr c, uint howmany=1) { return WordCounter::insert ( concat3(a,b,c), howmany ); }
		status remove_one (CStr a, CStr b, CStr c, uint howmany=1) { return WordCounter::remove_one ( concat3(a,b,c), howmany ); }
		status remove (CStr a, CStr b, CStr c) { return WordCounter::remove ( concat3(a,b,c) ); }
		Count count (CStr a, CStr b, CStr c, CStr d) { return WordCounter::count (concat4(a,b,c,d)); }
		status insert (CStr a, CStr b, CStr c, CStr d, uint howmany=1) { return WordCounter::insert ( concat4(a,b,c,d), howmany ); }
		status remove_one (CStr a, CStr b, CStr c, CStr d, uint howmany=1) { return WordCounter::remove_one ( concat4(a,b,c,d), howmany ); }
		status remove (CStr a, CStr b, CStr c, CStr d) { return WordCounter::remove ( concat4(a,b,c,d) ); }
		//status count_words_in_file  (istream& in);
};

typedef const WordPairCounter& WordPairCounterCR;
//typedef CounterTrie0Cursor WordCounterCursor;


//inline void write  (ostream& out, WordCounterCR theinfo) { write(out,theinfo,Format("T: ")); }
//inline status read  (istream& in, WordCounter& theinfo) { DOr(read (in,theinfo,Format("T: "))); theinfo.update_wordcount(); return OK; }



#endif
