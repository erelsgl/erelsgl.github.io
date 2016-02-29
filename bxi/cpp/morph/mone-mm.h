/*
 * File morph/mone-mm.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/mone-mm.h
 */

/* mone-mm.h -- word-word counter 
      (counts, for each word, the number of times each other word occurs after it).
*/

#ifndef __MONEMM_H
#define __MONEMM_H

#include <cpplib/wordcnt.h>
//#include <cpplib/mapc.hxx>
#include <cpplib/sikui.h>
#include <morph/morphinf.h>

/*        ***** USAGE EXAMPLE ***** (OUTDATED!)
// inserting and counting:
	MoneMilimAxreiMila yourcounter;
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



class MoneMilimAxreiMila;
typedef const MoneMilimAxreiMila& MoneMilimAxreiMilaCR;

typedef Trie0<WordCounter> TrieOfWordcounters ;

class MoneMilimAxreiMila {
	TrieOfWordcounters mytries[16];
	TrieOfWordcounters& trie (HeleqDiber hd) { return mytries[hd+8]; }
	const TrieOfWordcounters& trie (HeleqDiber hd) const { return mytries[hd+8]; }
public:
	void zero_all() {
		for (Index i=0; i<16; ++i) {
//			mytries[i].remove_all();
			for (TrieOfWordcounters::Cursor c(mytries[i]); c.isvalid(); ++c) 
				c.item().zero_all();
		}
	}
//	status setcount (MorphInfoCR theinfo, Sikui thesikui=0) {
//		return counter(theinfo.heleqdiber()).setcount (theinfo.baseword(), thesikui ); }
	WordCounter* milim_axrei_p (MorphInfoCR theinfo) {
		return trie(theinfo.heleqdiber()).itemp (theinfo.baseword()); }

	const WordCounter* milim_axrei_p (MorphInfoCR theinfo) const {
		return trie(theinfo.heleqdiber()).itemp (theinfo.baseword()); }

	status add (MorphInfoCR theinfo, MorphInfoCR theinfo_axrei, Sikui thesikui=SIKUI1) {
		TrieOfWordcounters& thetrie = trie(theinfo.heleqdiber());
		WordCounter* ma =  thetrie.itemp(theinfo.baseword());
		if (ma==NULL) {
			ma = new WordCounter;
			DOr (ma->insert (theinfo_axrei.mila_rijona(), thesikui));
			//writeln (thetrie);
			return thetrie.insert(theinfo.baseword(),*ma);
			//writeln (thetrie);
		}
		else
			return ma->insert (theinfo_axrei.mila_rijona(), thesikui);
		//writeln (thetrie);
	}

	Count count (MorphInfoCR theinfo, MorphInfoCR theinfo_axrei, Sikui thesikui=SIKUI1) const {
		const WordCounter* ma =  milim_axrei_p(theinfo);
		if (ma==NULL) return 0;
		else return ma->count (theinfo_axrei.mila_rijona());
	}

	void ktov_bituyim (ostream& out, uint the_saf) {
		for (Index i=0; i<16; ++i) {
			for (TrieOfWordcounters::Cursor c(mytries[i]); c.isvalid(); ++c) {
				for (WordCounter::Cursor cc (c.item()); cc.isvalid(); ++cc) {
					if (cc.count() >= the_saf)
						out << c.string() << " " << cc.data() << " " << cc.count() << endl;
				}
			}
		}
	}

	friend void write (ostream& out, MoneMilimAxreiMilaCR theinfo);
	friend status read (istream& in, MoneMilimAxreiMila& theinfo);
};

void write (ostream& out, MoneMilimAxreiMilaCR theinfo) {
	for (Index i=0; i<16; ++i) {
		write(out, theinfo.mytries[i], Format("T:L ")); 
	}
}

status read (istream& in, MoneMilimAxreiMila& theinfo) {
	for (Index i=0; i<16; ++i)
		DOr(read (in,theinfo.mytries[i], Format("T:L ") ));
	return OK;
}



#endif
