/*
 * File morph/sntncsik.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncsik.h
 */

/* sntncsik.h -- 
	Sentence Options with Sikuiim:
		a structure that holds all options to analyze a sentence, with "sikui" attached to 
		each option.
*/

#ifndef __SNTNCSIK_H
#define __SNTNCSIK_H

#include <morph/soi.h>

class SentenceOptionsWithSikui;
typedef const SentenceOptionsWithSikui& SentenceOptionsWithSikuiCR;

class SentenceOptionsWithSikui: public FuzzySetAsVector1<SentenceInfo>   {
	public:

		/************************* editing the options ************************/

		status append_option (SentenceInfoCR theinfo, double thesikui=1.) { // append a new option, with the given sikui 
			DOr(append_empty());
			last().data() = theinfo;
			last().key() = sikui(thesikui);
			normalize();
			return OK;
		}


		/******** constructors *************/
		SentenceOptionsWithSikui (Size thesize=0): 
			FuzzySetAsVector1<SentenceInfo> (thesize) {}
};


status convert_eachword_options_to_sentence_options (
	ArrayOfMOWS the_eachword_options,
	uint TNOSO,     // number of sentence-options
	SentenceOptionsWithSikui& the_sentence_options
);
/*
convert_eachword_options_to_sentence_options:
	INPUT:  1. an array of MOWS (one MOWS for each word of a sentence),
		    2. a number (TNOSO).
	OUTPUT: one SOWS, with the TNOSO most-probable options to analyse the sentence.
		(the options will be NOT-MUFRAD)
	SIDE-EFFECT: the MOWS in the array will be sorted by decreasing sikui.
	ALGORITHM: uses a queue to hold the current 'candidates' (see the implementation).
*/



inline void write (ostream& out, SentenceOptionsWithSikuiCR theinfo, Format format="") {
	write (out, (FuzzySetAsVector1<SentenceInfo>&)theinfo, Format("P\n")); }
inline status read  (istream& in, SentenceOptionsWithSikui& theinfo,Format format="") {
	return read (in, (FuzzySetAsVector1<SentenceInfo>&)theinfo, Format("P\n")); }


DEFINE_INLINE_IO_OPERATORS(SentenceOptionsWithSikui);


#endif
